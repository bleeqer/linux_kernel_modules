
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>

#define DEVICE_NAME "mychardev"

static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) 
{
    const char *msg = "Hello from kernel\n";
    size_t len = strlen(msg);

    if (*ppos >= len) {
        return 0;
    }

    if (count > len - *ppos) {
        count = len - *ppos;
    }

    if (copy_to_user(buf, msg + *ppos, count)) {
        return -EFAULT;
    }

    *ppos += count;
    return count;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    char kbuf[128];

    if (count > sizeof(kbuf) - 1) {
        count = sizeof(kbuf) - 1;
    }

    if (copy_from_user(kbuf, buf, count)) {
        return -EFAULT;
    }

    kbuf[count] = '\0';
    pr_info("mychardev: user wrote: %s\n", kbuf);

    return count;
}

static const struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
};

static struct miscdevice my_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &my_fops,
};

static int __init my_init(void)
{
    int ret = misc_register(&my_dev);

    if (ret) {
        pr_err("mychardev: failed to register\n");
    } else {
        pr_info("mychardev: loaded\n");
    }
    return ret;
}

static void __exit my_exit(void)
{
    misc_deregister(&my_dev);
    pr_info("mychardev: unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("linux-king");
MODULE_DESCRIPTION("Simple char device");

