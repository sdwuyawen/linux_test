/**
 *	@filename :
 * 		test_usb1.c
 * 
 * 	@note :
 * 		
 * 
 **/ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

#include <linux/usbdevice_fs.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 20)
#include <linux/usb/ch9.h>
#else
#include <linux/usb_ch9.h>
#endif
#include <asm/byteorder.h>

#include <sys/stat.h>

#define D		printf
#define DBGX	printf

static inline int badname(const char *name)
{
    while(*name) {
        if(!isdigit(*name++)) return 1;
    }
    return 0;
}

int is_adb_interface(int vid, int pid, int usb_class, int usb_subclass, int usb_protocol)
{
	if (usb_class == 255 && usb_subclass == 66 &&
			usb_protocol == 1) {
		return 1;
	}

    return 0;
}

static void find_usb_device(const char *base,
        void (*register_device_callback)
                (const char *, const char *, unsigned char, unsigned char, int, int, unsigned))
{
    char busname[32], devname[32];
    unsigned char local_ep_in, local_ep_out;
    DIR *busdir , *devdir ;
    struct dirent *de;
    int fd ;

    busdir = opendir(base);
    if(busdir == 0) return;

    while((de = readdir(busdir)) != 0) {
        if(badname(de->d_name)) continue;

        snprintf(busname, sizeof busname, "%s/%s", base, de->d_name);
        devdir = opendir(busname);
        if(devdir == 0) continue;

//        DBGX("[ scanning %s ]\n", busname);
        while((de = readdir(devdir))) {
            unsigned char devdesc[4096];
            unsigned char* bufptr = devdesc;
            unsigned char* bufend;
            struct usb_device_descriptor* device;
            struct usb_config_descriptor* config;
            struct usb_interface_descriptor* interface;
            struct usb_endpoint_descriptor *ep1, *ep2;
            unsigned zero_mask = 0;
            unsigned vid, pid;
            size_t desclength;

            if(badname(de->d_name)) continue;
            snprintf(devname, sizeof devname, "%s/%s", busname, de->d_name);

            //if(known_device(devname)) {
                //DBGX("skipping %s\n", devname);
                //continue;
            //}

//            DBGX("[ scanning %s ]\n", devname);
            if((fd = open(devname, O_RDONLY)) < 0) {
                continue;
            }

            desclength = read(fd, devdesc, sizeof(devdesc));
            bufend = bufptr + desclength;

                // should have device and configuration descriptors, and atleast two endpoints
            if (desclength < USB_DT_DEVICE_SIZE + USB_DT_CONFIG_SIZE) {
                D("desclength %d is too small\n", desclength);
                close(fd);
                continue;
            }

            device = (struct usb_device_descriptor*)bufptr;
            bufptr += USB_DT_DEVICE_SIZE;

            if((device->bLength != USB_DT_DEVICE_SIZE) || (device->bDescriptorType != USB_DT_DEVICE)) {
                close(fd);
                continue;
            }

            vid = device->idVendor;
            pid = device->idProduct;
            DBGX("[ %s is V:%04x P:%04x ]\n", devname, vid, pid);

                // should have config descriptor next
            config = (struct usb_config_descriptor *)bufptr;
            bufptr += USB_DT_CONFIG_SIZE;
            if (config->bLength != USB_DT_CONFIG_SIZE || config->bDescriptorType != USB_DT_CONFIG) {
                D("usb_config_descriptor not found\n");
                close(fd);
                continue;
            }

                // loop through all the descriptors and look for the ADB interface
            while (bufptr < bufend) {
                unsigned char length = bufptr[0];
                unsigned char type = bufptr[1];

                if (type == USB_DT_INTERFACE) {
                    interface = (struct usb_interface_descriptor *)bufptr;
                    bufptr += length;

                    if (length != USB_DT_INTERFACE_SIZE) {
                        D("interface descriptor has wrong size\n");
                        break;
                    }

                    DBGX("bInterfaceClass: %d,  bInterfaceSubClass: %d,"
                         "bInterfaceProtocol: %d, bNumEndpoints: %d\n",
                         interface->bInterfaceClass, interface->bInterfaceSubClass,
                         interface->bInterfaceProtocol, interface->bNumEndpoints);

                    if (interface->bNumEndpoints == 2 &&							
                            is_adb_interface(vid, pid, interface->bInterfaceClass,
                            interface->bInterfaceSubClass, interface->bInterfaceProtocol))  {
                        struct stat st;
                        char pathbuf[128];
                        char link[256];
                        char *devpath = NULL;

                        DBGX("looking for bulk endpoints\n");
                            // looks like ADB...
                        ep1 = (struct usb_endpoint_descriptor *)bufptr;
                        bufptr += USB_DT_ENDPOINT_SIZE;
                        ep2 = (struct usb_endpoint_descriptor *)bufptr;
                        bufptr += USB_DT_ENDPOINT_SIZE;

                        if (bufptr > devdesc + desclength ||
                            ep1->bLength != USB_DT_ENDPOINT_SIZE ||
                            ep1->bDescriptorType != USB_DT_ENDPOINT ||
                            ep2->bLength != USB_DT_ENDPOINT_SIZE ||
                            ep2->bDescriptorType != USB_DT_ENDPOINT) {
                            D("endpoints not found\n");
                            break;
                        }

                            // both endpoints should be bulk
                        if (ep1->bmAttributes != USB_ENDPOINT_XFER_BULK ||
                            ep2->bmAttributes != USB_ENDPOINT_XFER_BULK) {
                            D("bulk endpoints not found\n");
                            continue;
                        }
                            /* aproto 01 needs 0 termination */
                        if(interface->bInterfaceProtocol == 0x01) {
                            zero_mask = ep1->wMaxPacketSize - 1;
                        }

                            // we have a match.  now we just need to figure out which is in and which is out.
                        if (ep1->bEndpointAddress & USB_ENDPOINT_DIR_MASK) {
                            local_ep_in = ep1->bEndpointAddress;
                            local_ep_out = ep2->bEndpointAddress;
                        } else {
                            local_ep_in = ep2->bEndpointAddress;
                            local_ep_out = ep1->bEndpointAddress;
                        }

                            // Determine the device path
                        if (!fstat(fd, &st) && S_ISCHR(st.st_mode)) {
                            char *slash;
                            ssize_t link_len;
                            snprintf(pathbuf, sizeof(pathbuf), "/sys/dev/char/%d:%d",
                                     major(st.st_rdev), minor(st.st_rdev));
                            link_len = readlink(pathbuf, link, sizeof(link) - 1);
                            if (link_len > 0) {
                                link[link_len] = '\0';
                                slash = strrchr(link, '/');
                                if (slash) {
                                    snprintf(pathbuf, sizeof(pathbuf),
                                             "usb:%s", slash + 1);
                                    devpath = pathbuf;
                                }
                            }
                        }
						
						if (register_device_callback != NULL) {
							register_device_callback(devname, devpath,
                                local_ep_in, local_ep_out,
                                interface->bInterfaceNumber, device->iSerialNumber, zero_mask);
						}
                        break;
                    }
                } else {
                    bufptr += length;
                }
            } // end of while

            close(fd);
        } // end of devdir while
        closedir(devdir);
    } //end of busdir while
    closedir(busdir);
}

static void register_device(const char *dev_name, const char *devpath,
                            unsigned char ep_in, unsigned char ep_out,
                            int interface, int serial_index, unsigned zero_mask)
{
	printf("register adb device: %s\n", dev_name);
	printf("\tdevpath = %s\n", devpath);
	printf("\tep_in = %d\n", ep_in);
	printf("\tep_out = %d\n", ep_out);
	printf("\tinterface = %d\n", interface);
	printf("\tserial_index = %d\n", serial_index);
	printf("\tzero_mask = %d\n", zero_mask);
}

void main(void) 
{
	find_usb_device("/dev/bus/usb", register_device);
}
