/**
	@filename :
		myfifo.c
**/
#include <linux/module.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/poll.h>

#include <asm/io.h>
//#include <asm/system.h>
#include <asm/uaccess.h>

//使用中断机制添加头文件   
#include <linux/sched.h>   
#include <asm/signal.h>   
#include <linux/interrupt.h>   
  
//使用udev机制添加的头文件   
#include <linux/slab.h>   
  
#ifdef CONFIG_SLUB   
#include <linux/slub_def.h>   
#elif defined CONFIG_SLOB   
#include <linux/slob_def.h>   
#else   
#include <linux/slab_def.h>   
#endif   

#define MYFIFO_CLEAR			0x01

#define MYFIFO_SIZE				0x1000
#define MYFIFO_MAJOR			200

#define MYFIFO_REGION_NUM	1

struct myfifo_dev
{
    struct cdev cdev;    
    struct semaphore sem;	
	wait_queue_head_t outq;
	wait_queue_head_t inq;
	size_t size;
	unsigned char *wp;
	unsigned char *rp;
	unsigned char *end;
	unsigned char buffer[MYFIFO_SIZE];	
};

static int myfifo_major = MYFIFO_MAJOR;
static struct class *myfifo_class;  /*by paul.*/
static struct myfifo_dev *myfifo_devp;

static int myfifo_open(struct inode* inode, struct file * filp)
{
	struct myfifo_dev * dev;
	
	dev = container_of(inode->i_cdev, struct myfifo_dev, cdev);
	filp->private_data = dev;
	
	printk(KERN_EMERG "%s : %p\n", __FUNCTION__, dev);
	
    return 0;
}

static int myfifo_release(struct inode * inode, struct file * filp)
{
	struct myfifo_dev * dev;
	
	dev = container_of(inode->i_cdev, struct myfifo_dev, cdev);
	
	printk(KERN_EMERG "%s : %p\n", __FUNCTION__, dev);
	
    return 0;
}

#ifdef __x86_64__	
static long myfifo_ioctl(struct file * filp, unsigned int cmd, unsigned long arg)
#else
static int myfifo_ioctl(struct inode * inodep, struct file * filp, unsigned int cmd, unsigned long arg)
#endif
{
    struct myfifo_dev * dev = filp->private_data;
    
    switch(cmd)
    {            
		case MYFIFO_CLEAR:
			if (down_interruptible(&dev->sem))	
				return -ERESTARTSYS;	

			dev->rp = dev->buffer;
			dev->wp = dev->buffer;
			dev->end = dev->wp + MYFIFO_SIZE;
			dev->size = 0;

			up(&dev->sem); /*释放锁*/	
			break;
			
        default:
            return -EINVAL;
    }

    return 0;
}

static ssize_t myfifo_read(struct file * filp, char __user * buf, size_t size, loff_t *ppos)
{
    size_t count = size;
    int ret = 0;
    struct myfifo_dev *dev = filp->private_data;
	
	if (down_interruptible(&dev->sem))	
		 return -ERESTARTSYS;
	
	while (!dev->size) {/*无数据可读*/	
		up(&dev->sem); /*释放锁*/
		if (filp->f_flags & O_NONBLOCK)
		   return -EAGAIN;
		
		if (wait_event_interruptible(dev->inq, dev->size))	
			return -ERESTARTSYS;	
		
		if (down_interruptible(&dev->sem))	
			return -ERESTARTSYS;		
	}
	
	/*数据已就绪，返回*/	
	if (dev->wp > dev->rp) {
		count = min(count, (size_t)(dev->wp - dev->rp));
	} else {
		/*写入指针回卷，返回数据直到dev->end*/
		count = min(count, (size_t)(dev->end - dev->rp));
	}
	
	if (copy_to_user(buf, dev->rp, count)) {	
		up(&dev->sem);
		return -EFAULT;	
	}	
	
	dev->rp += count;	
	if (dev->rp == dev->end) {
		dev->rp = dev->buffer;/*回卷*/
	}	
	dev->size -= count;
	ret = count;	
	//printk(KERN_EMERG "%s : 0x%x\n", __FUNCTION__, dev->size);
	up(&dev->sem);
	
	/*最后，唤醒所有写入者并返回*/	
	wake_up_interruptible(&dev->outq);	

    return ret;
}

static unsigned int myfifo_poll(struct file *filp, poll_table *wait)
{
    struct myfifo_dev *dev = filp->private_data;
    unsigned int mask = 0;
    /*
    * The buffer is circular; it is considered full
    * if "wp" is right behind "rp" and empty if the
    * two are equal.
    */
	if (down_interruptible(&dev->sem)) {
		return -ERESTARTSYS;
	}
	
    poll_wait(filp, &dev->inq, wait);
    poll_wait(filp, &dev->outq, wait);
    if (dev->rp != dev->wp) {
		mask |= POLLIN | POLLRDNORM; /* readable */
    }
	
    if (dev->size < MYFIFO_SIZE) {
		mask |= POLLOUT | POLLWRNORM; /* writable */
    }
    up(&dev->sem);
	
    return mask;
}


static ssize_t myfifo_write(struct file * filp, const char __user *buf, size_t size, loff_t *ppos)
{
    size_t count = size;
    int ret = 0;
    struct myfifo_dev *dev = filp->private_data;

    if (down_interruptible(&dev->sem)) {
        return -ERESTARTSYS;
    }

	while (dev->size == MYFIFO_SIZE) {
		up(&dev->sem); /*释放锁*/
		if (filp->f_flags & O_NONBLOCK)
		   return -EAGAIN;
		
		if (wait_event_interruptible(dev->outq, (dev->size < MYFIFO_SIZE)))	
			return -ERESTARTSYS;	
		
		if (down_interruptible(&dev->sem))	
			return -ERESTARTSYS;			
	}

	if (dev->wp < dev->rp) {
		count = min(count, (size_t)(dev->rp - dev->wp));
	} else {
		/*写入指针回卷，返回数据直到dev->end*/
		count = min(count, (size_t)(dev->end - dev->wp));
	}
	
    if (copy_from_user(dev->wp, buf, count)) {
		up(&dev->sem);
		return -EFAULT;
    }
	
	dev->wp += count;	
	if (dev->wp == dev->end) {
		dev->wp = dev->buffer;/*回卷*/
	}
	dev->size += count;
	ret = count;	
	//printk(KERN_EMERG "%s : 0x%x\n", __FUNCTION__, dev->size);
    up(&dev->sem);

	wake_up_interruptible(&dev->inq);	

    return ret;
}

static loff_t myfifo_llseek(struct file * filp, loff_t offset, int orig)
{
	return -1;
}

static const struct file_operations myfifo_fops =
{
    .owner = THIS_MODULE,
    .llseek = myfifo_llseek,
    .read = myfifo_read,
    .write = myfifo_write,
#ifdef __x86_64__	
	.compat_ioctl = myfifo_ioctl,	//在64bit系统上，32bit的ioctl调用将使用此函数指针代替
#else
    .ioctl = myfifo_ioctl,
#endif        
    .open = myfifo_open,
    .write = myfifo_write,
    .poll = myfifo_poll,
    .release = myfifo_release,
};

static void myfifo_setup_cdev(struct myfifo_dev *dev, int index)
{
    int err;
    int devno = MKDEV(myfifo_major, index);

    cdev_init(&dev->cdev, &myfifo_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &myfifo_fops;
    err = cdev_add(&dev->cdev, devno, 1);
    if(err)
    {
        printk(KERN_EMERG "Error %d adding globalmem %d\n", err, index);
    }
}


int myfifo_init(void)
{
    int result;
    dev_t devno = MKDEV(myfifo_major, 0);

    if(myfifo_major)
    {
        result = register_chrdev_region(devno, MYFIFO_REGION_NUM, "myfifo");
    }
    else
    {
        result = alloc_chrdev_region(&devno, 0, MYFIFO_REGION_NUM, "myfifo");
        myfifo_major = MAJOR(devno);
    }	

    myfifo_devp = kmalloc(sizeof(struct myfifo_dev), GFP_KERNEL);
    if(!myfifo_devp)
    {		
        result = -ENOMEM;
        goto fail_malloc;
    }
    memset(myfifo_devp, 0, sizeof(struct myfifo_dev));

    myfifo_setup_cdev(myfifo_devp, 0);	

	/* create virtual_disk_class under /sysfs by paul.*/
	myfifo_class = class_create(THIS_MODULE, "myfifo");
	if(IS_ERR(myfifo_class))
	{
		printk("Err: failed in creating globalmeme_class.\n");
		cdev_del(&myfifo_devp->cdev);
		goto fail_malloc; 
	}
	/* register your own device in sysfs, and this will cause udev to create corresponding device node */
	device_create(myfifo_class, NULL, MKDEV(myfifo_major, 0), NULL, "myfifo");
	printk (KERN_INFO "Registered character driver myfifo.\n");	

	init_waitqueue_head(&myfifo_devp->inq);
	init_waitqueue_head(&myfifo_devp->outq);
	
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 24))
	sema_init(&myfifo_devp->sem, 1);
#else	
	init_MUTEX(&myfifo_devp->sem);
#endif	

	myfifo_devp->rp = myfifo_devp->buffer;
	myfifo_devp->wp = myfifo_devp->buffer;
	myfifo_devp->end = myfifo_devp->wp + MYFIFO_SIZE;
	myfifo_devp->size = 0;
	
	return 0;

fail_malloc:
	if (myfifo_devp)
	{
		kfree(myfifo_devp);
		myfifo_devp = NULL;
	}	
	
	unregister_chrdev_region(devno, MYFIFO_REGION_NUM);
	
	return result;
}

static void myfifo_exit(void)
{	
	cdev_del(&myfifo_devp->cdev);

	device_destroy(myfifo_class, MKDEV(myfifo_major, 0)); 
	class_destroy(myfifo_class);  	

    kfree(myfifo_devp);	

	unregister_chrdev_region(MKDEV(myfifo_major, 0), MYFIFO_REGION_NUM);

	printk (KERN_INFO "Unregistered character driver myfifo.\n");
}

MODULE_AUTHOR("paul");
MODULE_LICENSE("Dual BSD/GPL");

module_init(myfifo_init);
module_exit(myfifo_exit);

