#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static dev_t               dev_nr;
static struct cdev         sig_cdev;
static struct class*       dev_class;
static struct device*      sig_dev;
static struct task_struct* blink_thread;

static int blink_thread_func(void* params)
{
    int i = 0;
    while(!kthread_should_stop())
    {
        pr_info("Hello from blink_thread: %d\n", i++);
        msleep(1000);
    }
    return 0;
}

static int sig_open(struct inode* dev_file, struct file* instance)
{
    pr_info("signalru: Device File opened ...\n");
    return 0;
}

static int sig_close(struct inode* dev_file, struct file* instance)
{
    pr_info("signalru: Device File closed ...\n");
    return 0;
}

static ssize_t sig_read(struct file* instance, char __user* user, size_t count, loff_t* offset)
{
    pr_info("signalru: sig_read start ...\n");
    return 0;
}

static ssize_t sig_write(struct file* instance, const char __user* user, size_t count, loff_t* offset)
{
    pr_info("signalru: sig_write start ...\n");
    return count;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .read    = sig_read,
    .write   = sig_write,
    .open    = sig_open,
    .release = sig_close,
};

static int __init sig_driver_init(void)
{
    pr_info("signalru: sig_driver_init start ...\n");

    // Allocate major number
    if(alloc_chrdev_region(&dev_nr, 0, 1, "sig_dev") < 0)
    {
        pr_err("signalru: alloc_chrdev_region failed ...\n");
        return -EIO;
    }
    pr_info("signalru: Major = %d, Minor = %d  ...\n", MAJOR(dev_nr), MINOR(dev_nr));

    // Create cdev struct
    cdev_init(&sig_cdev, &fops);

    // Add character device
    if(cdev_add(&sig_cdev, dev_nr, 1) < 0)
    {
        pr_err("signalru: cdev_add failed ...\n");
        goto free_class;
    }

    // Create struct class
    if(IS_ERR(dev_class = class_create("sig_class")))
    {
        pr_err("signalru: class_create failed ...\n");
        goto free_class;
    }

    // Create device
    if(IS_ERR(sig_dev = device_create(dev_class, NULL, dev_nr, NULL, "%s", "led_on_off_ru")))
    {
        pr_err("signalru: device_create failed ...\n");
        goto free_device;
    }

    // Create kernel thread
    blink_thread = kthread_run(blink_thread_func, NULL, "blink_thread");
    if(blink_thread)
    {
        pr_info("signalru: Created kernel thread ...\n");
    }
    else
    {
        pr_err("signalru: Failed to create kernel thread ...\n");
        goto free_device;
    }

    pr_info("signalru: sig_driver_init finished ...\n");
    return 0;

free_device:
    class_destroy(dev_class);
free_class:
    unregister_chrdev_region(dev_nr, 1);
    cdev_del(&sig_cdev);

    return -EIO;
}

static void __exit sig_driver_exit(void)
{
    pr_info("signalru: sig_driver_exit start ...\n");

    // Kill thread
    kthread_stop(blink_thread);

    // Delete sysfs entry and device file
    device_destroy(dev_class, dev_nr);
    class_destroy(dev_class);

    // Remove driver
    cdev_del(&sig_cdev);
    unregister_chrdev_region(dev_nr, 1);

    pr_info("signalru: sig_driver_exit finished ...\n");
    return;
}

module_init(sig_driver_init);
module_exit(sig_driver_exit);

MODULE_AUTHOR("Tim Krueger");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A driver to toggle GPIO Pin 23 in a kernel thread.");
MODULE_VERSION("0.01");
