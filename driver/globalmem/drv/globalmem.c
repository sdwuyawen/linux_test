/**
	@filename :
		globalmem.c
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
#include <linux/sysfs.h>
#include <linux/platform_device.h>

#ifdef CONFIG_PM
#include <linux/pm.h>
#endif

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

#define GLOBALMEM_SIZE 0x1000 /*4KB*/
#define MEM_CLEAR 0x1  /*clear globale memory*/
#define GLOBALMEM_MAJOR 199 /**/

#define GLOBALMEM_IRQ_EN

#define GLOBALMEM_REGION_NUM	2

struct globalmem_dev
{	
	char name[16];
	int id;
    struct cdev cdev;
    struct bin_attribute bin;
    unsigned char mem[GLOBALMEM_SIZE];
    struct semaphore sem;
	unsigned int cnt;
	int data;
};

static int globalmem_major = GLOBALMEM_MAJOR;
static struct class *globalmeme_class;  /*by paul.*/
static struct globalmem_dev *g_globalmem_devp;

#ifdef GLOBALMEM_IRQ_EN
static unsigned int mems_irq = 1;  
#endif
  
//module_param()添加模块支持参数，每行设置一个参数
module_param(globalmem_major, int, S_IRUGO);
#ifdef GLOBALMEM_IRQ_EN
module_param(mems_irq, int, S_IRUGO);  
#endif

static ssize_t demo_class_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{	
	struct globalmem_dev *globalmem_devp = dev_get_drvdata(dev);
	unsigned long data;
	int error;	
	
	printk("%s : dev=%p, %p\n", __FUNCTION__, dev, globalmem_devp);

	error = strict_strtoul(buf, 10, &data);
	if (error) {
		return error;
	}
	
	globalmem_devp->data = data;
		
	return count;
}

static ssize_t demo_class_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct globalmem_dev *globalmem_devp = dev_get_drvdata(dev);
	printk("%s : dev=%p, %p\n", __FUNCTION__, dev, globalmem_devp);
	
	return sprintf(buf, "%d\n", globalmem_devp->data);
}

DEVICE_ATTR(demo_class_data, S_IRUGO | S_IWUSR, demo_class_show, demo_class_store);

static struct attribute *demo_class_attrs[] = {
	&dev_attr_demo_class_data.attr, 
	NULL,
};

ATTRIBUTE_GROUPS(demo_class);

static ssize_t demo_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{	
	struct globalmem_dev *globalmem_devp = dev_get_drvdata(dev);
	unsigned long data;
	int error;	
	
	printk("%s : dev=%p, %p, %s\n", __FUNCTION__, dev, globalmem_devp, globalmem_devp->name);

	error = strict_strtoul(buf, 10, &data);
	if (error) {
		return error;
	}
	
	globalmem_devp->data = data;
		
	return count;
}

static ssize_t demo_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct globalmem_dev *globalmem_devp = dev_get_drvdata(dev);
	
	printk("%s : dev=%p, %p, %s\n", __FUNCTION__, dev, globalmem_devp, globalmem_devp->name);
	
	return sprintf(buf, "%d\n", globalmem_devp->data);
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34))
static ssize_t demo_bin_read(struct file *filp, struct kobject *kobj,
		struct bin_attribute *attr,
		char *buf, loff_t off, size_t count)
#else
static ssize_t demo_bin_read(struct kobject *kobj,
		struct bin_attribute *attr,
		char *buf, loff_t off, size_t count)
#endif		
{
	struct globalmem_dev *globalmem_devp = dev_get_drvdata(container_of(kobj, struct device, kobj));
	int end;
	
	printk("%s : off=0x%llx, 0x%lx, %s\n", __FUNCTION__, off, count, globalmem_devp->name);
	
	end = off + count; 
	if (end > GLOBALMEM_SIZE)
	{
		count -= (end - GLOBALMEM_SIZE);
	}	
	
	memcpy(buf, globalmem_devp->mem, count);
	
	return count;
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34))
static ssize_t demo_bin_write(struct file *filp, struct kobject *kobj,
		struct bin_attribute *attr,
		char *buf, loff_t off, size_t count)
#else
static ssize_t demo_bin_write(struct kobject *kobj,
		struct bin_attribute *attr,
		char *buf, loff_t off, size_t count)
#endif		
{
	struct globalmem_dev *globalmem_devp = dev_get_drvdata(container_of(kobj, struct device, kobj));
	int end;
	
	printk("%s : off=0x%llx, 0x%lx, %s\n", __FUNCTION__, off, count, globalmem_devp->name);
	
	end = off + count; 
	if (end > GLOBALMEM_SIZE)
	{
		count -= (end - GLOBALMEM_SIZE);
	}
	
	memcpy(globalmem_devp->mem, buf, count);
	
	return count;
}


static DEVICE_ATTR(demo_data, S_IWUSR | S_IRUGO, demo_show, demo_store);

static struct attribute *demo_attributes[] = {
    &dev_attr_demo_data.attr,
    NULL
};

static const struct attribute_group demo_attr_group = {
    .attrs = demo_attributes,
};

static void demo_dev_release(struct device *dev)
{
	struct globalmem_dev *globalmem_devp = dev_get_drvdata(dev);
	
	printk("%s : dev=%p, %p %s\n", __FUNCTION__, dev, globalmem_devp, globalmem_devp->name);
	
	return;
}

static struct platform_device demo_dev[GLOBALMEM_REGION_NUM];

#ifdef GLOBALMEM_IRQ_EN
//对应的中断向量的ISR   
static irqreturn_t mems_irq_handler(int irq,void *dev_id)   
{  
    printk(KERN_INFO "Interrupt handler...............................%d\n",irq);  
    return IRQ_HANDLED;  
}
#endif

static int demo_drv_remove(struct platform_device *dev)
{
	struct globalmem_dev *globalmem_devp = platform_get_drvdata(dev);    
	
	sysfs_remove_bin_file(&dev->dev.kobj, &globalmem_devp->bin);
	sysfs_remove_group(&dev->dev.kobj, &demo_attr_group);	
	
	printk("%s : %s\n", __FUNCTION__, globalmem_devp->name);	
    cdev_del(&globalmem_devp->cdev);
	device_destroy(globalmeme_class, MKDEV(globalmem_major, dev->id)); 
	
#ifdef GLOBALMEM_IRQ_EN	
    //释放设备号和中断向量   
    free_irq(mems_irq, globalmem_devp);   	
#endif
	
	return 0;
}

static int globalmem_open(struct inode* inode, struct file * filp)
{
	struct globalmem_dev * dev;

	dev = container_of(inode->i_cdev, struct globalmem_dev, cdev);
	filp->private_data = dev;
	
	if(down_interruptible(&dev->sem))
	{
		return -ERESTARTSYS;
	}
	
	dev->cnt++;
	up(&dev->sem);

	printk(KERN_EMERG "%s : %p, %d\n", __FUNCTION__, dev, dev->cnt);
	
    return 0;
}

static int globalmem_release(struct inode * inode, struct file * filp)
{
	struct globalmem_dev * dev;

	dev = container_of(inode->i_cdev, struct globalmem_dev, cdev);
	if(down_interruptible(&dev->sem))
	{
		return -ERESTARTSYS;
	}
		
	dev->cnt--;
	up(&dev->sem);
	
	printk(KERN_EMERG "%s : %p, %d\n", __FUNCTION__, dev, dev->cnt);
    return 0;
}

#ifdef __x86_64__	
static long globalmem_ioctl(struct file * filp, unsigned int cmd, unsigned long arg)
#else
static int globalmem_ioctl(struct inode * inodep, struct file * filp, unsigned int cmd, unsigned long arg)
#endif
{
    struct globalmem_dev * dev = filp->private_data;
    
    switch(cmd)
    {
        case MEM_CLEAR:
            if(down_interruptible(&dev->sem))
            {
                return -ERESTARTSYS;
            }
			
            memset(dev->mem, 0, GLOBALMEM_SIZE);		
			
            up(&dev->sem);
            printk(KERN_INFO "globalmem is set to 0\n");
            break;
            
        default:
            return -EINVAL;
    }

    return 0;
}

static ssize_t globalmem_read(struct file * filp, char __user * buf, size_t size, loff_t *ppos)
{
    unsigned int p = *ppos;
    unsigned int count = size;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;

    if(p >= GLOBALMEM_SIZE)
    {
        return count ? -ENXIO:0;
    }
    
    if(count > GLOBALMEM_SIZE - p)
    {
        count = GLOBALMEM_SIZE - p;
    }
	
    if(down_interruptible(&dev->sem))
    {
        return -ERESTARTSYS;
    }

    if(copy_to_user(buf, (void*)(dev->mem + p), count))
    {
        ret = -EFAULT;
    }
    else
    {
        *ppos += count;
        ret = count;
        printk(KERN_EMERG "read %d byte(s) from %d\n", count, p);        
    }

    up(&dev->sem);

    return ret;
}

static ssize_t globalmem_write(struct file * filp, const char __user *buf, size_t size, loff_t *ppos)
{
    unsigned int p = *ppos;
    unsigned int count = size;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;

    if(p >= GLOBALMEM_SIZE)
    {
        return (count ? -ENXIO : 0);
    }
    if(count > GLOBALMEM_SIZE - p)
    {
        count = GLOBALMEM_SIZE - p;
    }

    if(down_interruptible(&dev->sem))
    {
        return -ERESTARTSYS;
    }

    if(copy_from_user(dev->mem + p, buf, count))
    {
        ret = -EFAULT;
    }
    else
    {
        *ppos += count;
        ret = count;

        printk(KERN_EMERG "written %d byte(s) from %d\n", count, p);
    }

    up(&dev->sem);

    return ret;
}

static loff_t globalmem_llseek(struct file * filp, loff_t offset, int orig)
{
    loff_t ret = 0;

    switch(orig)
    {
        case 0:
            if(offset < 0)
            {
                ret = -EINVAL;
                break;
            }
            if((unsigned int)offset > GLOBALMEM_SIZE)
            {
                ret = -EINVAL;
                break;
            }
            filp->f_pos = (unsigned int)offset;
            ret = filp->f_pos;
            break;
        case 1:
            if((filp->f_pos + offset) > GLOBALMEM_SIZE)
            {
                ret = -EINVAL;
                break;
            }
            if((filp->f_pos + offset) < 0)
            {
                ret = -EINVAL;
                break;
            }
            filp->f_pos += offset;
            ret = filp->f_pos;
            break;
    }

    return ret;
}

static const struct file_operations globalmem_fops =
{
    .owner = THIS_MODULE,
    .llseek = globalmem_llseek,
    .read = globalmem_read,
    .write = globalmem_write,
#ifdef __x86_64__	
	.compat_ioctl = globalmem_ioctl,	//在64bit系统上，32bit的ioctl调用将使用此函数指针代替
#else
    .ioctl = globalmem_ioctl,
#endif        
    .open = globalmem_open,    
    .write = globalmem_write,
    .release = globalmem_release,
};

static void globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
    int err;
    int devno = MKDEV(globalmem_major, index);

    cdev_init(&dev->cdev, &globalmem_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &globalmem_fops;
    err = cdev_add(&dev->cdev, devno, 1);
    if(err)
    {
        printk(KERN_EMERG "Error %d adding globalmem %d\n", err, index);
    }
}

static int demo_drv_probe(struct platform_device *dev)
{
    int flagnum;     	
	struct globalmem_dev *globalmem_devp = dev->dev.platform_data;
	printk("%s\n", __FUNCTION__);
    
    platform_set_drvdata(dev, globalmem_devp);    

    globalmem_setup_cdev(globalmem_devp, dev->id);
		
	printk("dev[%d]=%p, %p, %s\n", dev->id, &dev, &dev->dev, globalmem_devp->name);
	/* register your own device in sysfs, and this will cause udev to create corresponding device node */
	device_create(globalmeme_class, &dev->dev, MKDEV(globalmem_major, dev->id), globalmem_devp, "%s", globalmem_devp->name);
	printk (KERN_INFO "Registered character driver globalmeme = %p.\n", globalmem_devp);
	
#ifdef GLOBALMEM_IRQ_EN
 	//register interrupt.       
    if(!(flagnum = request_irq(mems_irq, mems_irq_handler, IRQF_SHARED, "globalmem", globalmem_devp)))  
    {  
        printk(KERN_INFO "Register interrupt correctly..\n");          
    }  
    else  
    {
        printk(KERN_INFO "Error was happened here...%d\n",flagnum);
		cdev_del(&globalmem_devp->cdev);
		goto fail_malloc;  
    }
#endif	
	
	sysfs_create_group(&dev->dev.kobj, &demo_attr_group);
	
	//sysfs_bin_attr_init(&globalmem_devp->bin);
	globalmem_devp->bin.attr.name = "eeprom";
	globalmem_devp->bin.attr.mode = 0666;
	globalmem_devp->bin.read = demo_bin_read;
	globalmem_devp->bin.write = demo_bin_write;
	globalmem_devp->bin.mmap = NULL;
	globalmem_devp->bin.size = GLOBALMEM_SIZE;
	sysfs_create_bin_file(&dev->dev.kobj, &globalmem_devp->bin);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 24))
    sema_init(&globalmem_devp->sem, 1);
#else
    init_MUTEX(&globalmem_devp->sem);
#endif	
	return 0;
	
fail_malloc:	
	
	return -1;	
}

static struct platform_driver demo_dev_drv = {
	.probe		= demo_drv_probe,
	.remove		= demo_drv_remove,		
	.driver = {
		.owner = THIS_MODULE,
		.name = "demoDev",				
	},
};

static int globalmem_init(void)
{
	int i;
    int result;
	dev_t devno;	
	
	printk (KERN_INFO "%s.\n", __FUNCTION__);

	devno = MKDEV(globalmem_major, 0);
    if(globalmem_major) {
        result = register_chrdev_region(devno, GLOBALMEM_REGION_NUM, "globalmem");
    } else {
        result = alloc_chrdev_region(&devno, 0, GLOBALMEM_REGION_NUM, "globalmem");
        globalmem_major = MAJOR(devno);
    }		
	
	/* create virtual_disk_class under /sysfs by paul.*/
	globalmeme_class = class_create(THIS_MODULE, "paul");
	//globalmeme_class->dev_attrs = demo_class_attrs;	 
	globalmeme_class->dev_groups = demo_class_groups;   	
	
	g_globalmem_devp = kmalloc(GLOBALMEM_REGION_NUM*sizeof(struct globalmem_dev), GFP_KERNEL);
	memset(g_globalmem_devp, 0, GLOBALMEM_REGION_NUM*sizeof(struct globalmem_dev));
	for (i=0; i<GLOBALMEM_REGION_NUM; i++)
	{
		sprintf(g_globalmem_devp[i].name, "globalmem%d", i+1);
		g_globalmem_devp[i].id = i;
		demo_dev[i].name = "demoDev";		//match driver.name
		demo_dev[i].id = i;
		demo_dev[i].dev.platform_data = &g_globalmem_devp[i];
		demo_dev[i].dev.release       = demo_dev_release;		
		platform_device_register(&demo_dev[i]);		
	}
		
	platform_driver_register(&demo_dev_drv);		

    return 0;
}

static void globalmem_exit(void)
{	
	int i;
	
    platform_driver_unregister(&demo_dev_drv);
    
    for (i=0; i<GLOBALMEM_REGION_NUM; i++)
    {
		platform_device_unregister(&demo_dev[i]);
	}
	
	class_destroy(globalmeme_class);  	
		
	kfree(g_globalmem_devp);
	
    unregister_chrdev_region(MKDEV(globalmem_major, 0), GLOBALMEM_REGION_NUM);	
	
    printk (KERN_INFO "%s.\n", __FUNCTION__);
}

MODULE_AUTHOR("paul");
MODULE_LICENSE("Dual BSD/GPL");

module_init(globalmem_init);
module_exit(globalmem_exit);

