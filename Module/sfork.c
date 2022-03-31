#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/mm.h>
#include<linux/mm_types.h>
#include<linux/file.h>
#include<linux/fs.h>
#include<linux/path.h>
#include<linux/slab.h>
#include<linux/dcache.h>
#include<linux/sched.h>
#include<linux/uaccess.h>
#include<linux/fs_struct.h>
#include <asm/tlbflush.h>
#include<linux/uaccess.h>
#include<linux/device.h>

#define CHAR_DEVNAME "sfork_dev"

static int major;
atomic_t  device_opened;
static struct class *sfork_class;
struct device *sfork_device;

// static unsigned long gptr;

static int sfork_open(struct inode *inode, struct file *file)
{
        atomic_inc(&device_opened);
        try_module_get(THIS_MODULE);
        printk(KERN_INFO "Device opened successfully\n");
        return 0;
}

static int sfork_release(struct inode *inode, struct file *file)
{
        atomic_dec(&device_opened);
        module_put(THIS_MODULE);
        printk(KERN_INFO "Device closed successfully\n");
        return 0;
}

static ssize_t sfork_read(struct file *filp, char *buffer, size_t length, loff_t * offset)
{          
        return 0; 
        // printk(KERN_INFO "In read\n");
        // if (copy_to_user(buffer,&gptr,sizeof(unsigned long)) == 0)
        //      return sizeof(unsigned long);
        // return -1;
}

static ssize_t sfork_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
        return 0;
        // printk(KERN_INFO "In write\n");
        // if(copy_from_user(&gptr,buff,sizeof(unsigned long)) == 0){
        //     printk("The copied value is: %ld",gptr);
        //     return sizeof(unsigned long);
        // }
        // return -1;
}

static struct file_operations fops = {
        .read = sfork_read,
        .write = sfork_write,
        .open = sfork_open,
        .release = sfork_release,
};

static char *sfork_devnode(struct device *dev, umode_t *mode)
{
        if (mode && dev->devt == MKDEV(major, 0))
                *mode = 0666;
        return NULL;
}

int init_module(void)
{
        int err;
	    printk(KERN_INFO "Loading sfork module.\n");    
        major = register_chrdev(0, CHAR_DEVNAME, &fops);
        err = major;
        if (err < 0) {      
             printk(KERN_ALERT "Registering char device failed with %d\n", major);   
             goto error_regdev;
        }
        
        sfork_class = class_create(THIS_MODULE, CHAR_DEVNAME);
        err = PTR_ERR(sfork_class);
        if (IS_ERR(sfork_class))
                goto error_class;

        sfork_class->devnode = sfork_devnode;

        sfork_device = device_create(sfork_class, NULL, MKDEV(major, 0), NULL, CHAR_DEVNAME);
        err = PTR_ERR(sfork_device);
        if (IS_ERR(sfork_device))
                goto error_device;
 
        printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
        atomic_set(&device_opened, 0);
       

	return 0;

error_device:
         class_destroy(sfork_class);
error_class:
        unregister_chrdev(major, CHAR_DEVNAME);
error_regdev:
        return  err;
}

void cleanup_module(void)
{
    device_destroy(sfork_class, MKDEV(major, 0));
    class_destroy(sfork_class);
    unregister_chrdev(major, CHAR_DEVNAME);
	printk(KERN_INFO "Goodbye kernel\n");
}

MODULE_AUTHOR("chinmayg@cse.iitk.ac.in (TEAM RPC)");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Sfork modules");
