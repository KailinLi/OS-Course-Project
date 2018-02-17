#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/uaccess.h> 
 
#if CONFIG_MODVERSIONS == 1 
#define MODVERSIONS 
#include <linux/version.h> 
#endif 

#define DEVICE_NUM 0
 
static int device_num = 0;
static char buffer[1024] = "mydev";  
static int open_nr = 0;

static int mydev_open(struct inode *inode, struct file *filp); 
static int mydev_release(struct inode *inode, struct file* filp); 
static ssize_t mydev_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos); 
static ssize_t mydev_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos); 
 
static struct file_operations mydev_fops = {
    .owner = THIS_MODULE,
    .read = mydev_read, 
    .write = mydev_write, 
    .open = mydev_open, 
    .release = mydev_release, 
};    
 
static int mydev_open(struct inode *inode, struct file *filp) 
{ 
    printk("\nMain device is %d, and the slave device is %d\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev)); 
    if (open_nr == 0) { 
        open_nr++;
        try_module_get(THIS_MODULE);
        return 0;
    }  
    else { 
        printk(KERN_ALERT "Another process open the char device.\n");
        return -1;
    }  
} 
 
static ssize_t mydev_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos) 
{ 
    if (copy_to_user(buf, buffer, sizeof(buffer))) { 
        return -1; 
    } 
    return sizeof(buffer); 
} 
 
static ssize_t mydev_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos) 
{ 
    if (copy_from_user(buffer, buf, sizeof(buffer))) { 
        return -1; 
    } 
    return sizeof(buffer); 
} 
 
static int mydev_release(struct inode *inode, struct file* filp) 
{ 
    open_nr--;
    printk("The device is released!\n"); 
    module_put(THIS_MODULE); 
        return 0; 
} 
 
static int __init mydev_init(void) 
{ 
    int result; 
 
    printk(KERN_ALERT "Begin to init Char Device!");
    result = register_chrdev(DEVICE_NUM, "mydev", &mydev_fops); 
 
    if (result < 0) { 
        printk(KERN_WARNING "mydev: register failure\n"); 
        return -1; 
    } 
    else { 
        printk("mydev: register success!\n"); 
        device_num = result; 
        return 0; 
    } 
} 
 
static void __exit mydev_exit(void) 
{ 
    printk(KERN_ALERT "Unloading...\n"); 
    unregister_chrdev(device_num, "mydev");
    printk("unregister success!\n"); 
} 
 
module_init(mydev_init); 
module_exit(mydev_exit); 

MODULE_LICENSE("GPL"); 