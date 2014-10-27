ifeq ($(KERNELRELEASE),)

PWD :=$(shell pwd)
KERSRC := /lib/modules/$(shell uname -r)/build/

.PHONY: modules modules_install clean

modules:
	$(MAKE) -C $(KERSRC) M=$(PWD) modules
	
moules_install:
	$(MAKE) -C $(KERSRC) M=$(PWD) modules_install
	
clean:
	-rm -rf *.o *.cmd.* *.ko
	
else

modules-objs := mmap.o
obj-m := mmap.o

endif
