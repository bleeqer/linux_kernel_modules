#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("linux-king");
MODULE_DESCRIPTION("My null");

static int major;

static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    return count;
}

static int my_open(struct inode *inode, struct file *flip) 
{
    return 0;
}

static long int my_read(struct file *file, char __user *buf, long unsigned int count, long long int *ppops)
{
    return 0;
}

static struct file_operations fops = {
    .open = my_open,
    .read = my_read,
    .write = my_write,
};

static int __init chr_dev_init(void) 
{
    major = register_chrdev(0, "dev/my_null", &fops);
    if (major < 0) {
        printk(KERN_INFO "Something went wrong!");
    }
    
    printk(KERN_INFO "Device driver successfully loaded with major: %d", major);
    
    return 0;
}

static void __exit chr_dev_exit(void)
{
    unregister_chrdev(major, "/dev/my_null");
}
module_init(chr_dev_init);
module_exit(chr_dev_exit);
