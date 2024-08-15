#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 32

static dev_t               dev_nr;
static struct cdev         sig_cdev;
static struct class*       dev_class;
static struct device*      sig_dev;
static struct task_struct* blink_thread;
static struct gpio_desc*   gpio_desc;

static long int            freqKrueger, access_counter;
static char                freq_buffer[BUFFER_SIZE];
static bool                new_input = false;
static wait_queue_head_t   freq_wq;

static DEFINE_SPINLOCK(freq_lock);

static int blink_thread_func(void* params)
{
    int ret = 0, value = 0;
    long int provided_freq = 0, sleep_msecs = 1000;
    bool led_on = false;

    while(!kthread_should_stop())
    {
        ret = wait_event_interruptible_timeout
              ( freq_wq,
                ({
                   spin_lock(&freq_lock);
                   int cond = (new_input == true);
                   provided_freq = freqKrueger;
                   spin_unlock(&freq_lock);
                   cond;
                }),
                msecs_to_jiffies(sleep_msecs)
              );

        // Timeout reached
        if (ret == 0)
        {
            // If LED is activated, toggle it accordingly
            if(led_on == true)
            {
                value ^= 1;
                gpiod_set_value(gpio_desc, value);
            }

            // pr_info("signalru: timeout reached ...\n");
        }
        // Signal came in
        else if (ret == -ERESTARTSYS)
        {
            pr_info("signalru: interrupted by signal ...\n");
        }
        // Condition is true
        else
        {
            // Turn off LED
            if(provided_freq == 0)
            {
                led_on = false;
                gpiod_set_value(gpio_desc, 0);
            }
            // Turn on LED and calculate new frequency aka sleep time
            else
            {
                led_on = true;
                sleep_msecs = 1000 / provided_freq;
            }

            // Reset state (no new user input is yet provided)
            spin_lock(&freq_lock);
            new_input = false;
            spin_unlock(&freq_lock);

            // pr_info("signalru: condition == true ...\n");
        }
    }

    return 0;
}

static int config_gpios(void)
{
    pr_info("signalru: config_gpios start ...\n");

    struct device_node* nodeptr = sig_dev->of_node;
    nodeptr                     = of_find_node_by_name(nodeptr, "custom_gpio");
    sig_dev->of_node            = nodeptr;

    gpio_desc = gpiod_get(sig_dev, "my", GPIOD_OUT_LOW);
    if(IS_ERR(gpio_desc))
    {
        pr_err("signalru: gpiod_get failed ...\n");
        return -EIO;
    }

    pr_info("signalru: gpios reserved and configured ...\n");
    return 0;
}

static int free_gpios(void)
{
    if(gpio_desc)
    {
        gpiod_set_value(gpio_desc, 0);
        gpiod_put(gpio_desc);
        pr_info("signalru: gpios cleared up ...\n");
    }

    return 0;
}

static void validate_user_input(char* buffer, unsigned long buf_size)
{
    long int user_input;

    // Convert char array into long int
    if(kstrtol(buffer, 10, &user_input) == -EINVAL)
    {
        pr_info("signalru: error converting user input ...\n");
        return;
    }

    pr_info("signalru: converted user input (value = %ld) ...\n", user_input);

    // Log a message depending on user input
    if(user_input == 0)
    {
        pr_info("signalru: turn off LED ...\n");
    }
    else if(user_input > 0 && user_input <= 10)
    {
        pr_info("signalru: turn on LED (Frequency: %ld Hz) ...\n", user_input);
    }
    else
    {
        pr_info("signalru: please provide a value between 1-10 Hz or 0 for turning off ...\n");
        return;
    }

    // Enter critical section: Set global variable to provided user input
    spin_lock(&freq_lock);
    freqKrueger = user_input;
    new_input = true;
    spin_unlock(&freq_lock);

    // Wake up thread
    wake_up_interruptible(&freq_wq);
}

static int sig_open(struct inode* dev_file, struct file* instance)
{
    pr_info("signalru: Device File opened ...\n");
    access_counter = 0;
    return 0;
}

static int sig_close(struct inode* dev_file, struct file* instance)
{
    pr_info("signalru: Device File closed ...\n");
    return 0;
}

static ssize_t sig_read(struct file* instance, char __user* user, size_t count, loff_t* offset)
{
    // To only return something once per opening
    if(access_counter != 0)
    {
        return 0;
    }
    access_counter++;

    pr_info("signalru: sig_read start ...\n");

    unsigned long to_copy = BUFFER_SIZE, not_copied, delta;
    long int value;
    memset(freq_buffer, 0, BUFFER_SIZE);

    // Enter critical section: Get global variable
    spin_lock(&freq_lock);
    value = freqKrueger;
    spin_unlock(&freq_lock);

    // Get string representation of the value
    int str_len = snprintf(freq_buffer, to_copy, "%ld\n", value);

    // If convertion succeeded
    if(str_len >= 0 && str_len < to_copy)
    {
        // Copy data to user
        not_copied = copy_to_user(user, freq_buffer, to_copy);

        // Calculate delta
        delta = to_copy - not_copied;
        pr_info("signalru: bytes copied = %ld (%ld bytes were not copied) ...\n", delta, not_copied);
        *offset += delta;

        return delta;
    }

    return -EIO;
}

static ssize_t sig_write(struct file* instance, const char __user* user, size_t count, loff_t* offset)
{
    pr_info("signalru: sig_write start ...\n");

    unsigned long to_copy, not_copied, delta;
    memset(freq_buffer, 0, BUFFER_SIZE);

    // Get amount of bytes to copy
    to_copy = min(count, BUFFER_SIZE);

    // Copy data from user
    not_copied = copy_from_user(freq_buffer, user, to_copy);

    // Calculate delta
    delta = to_copy - not_copied;
    pr_info("signalru: bytes copied = %ld (%ld bytes were not copied) ...\n", delta, not_copied);
    *offset += delta;

    // Validate user input and set global value accordingly
    validate_user_input(&freq_buffer[0], BUFFER_SIZE);

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
        goto free_cdev;
    }

    // Create struct class
    if(IS_ERR(dev_class = class_create("sig_class")))
    {
        pr_err("signalru: class_create failed ...\n");
        goto free_cdev;
    }

    // Create device
    if(IS_ERR(sig_dev = device_create(dev_class, NULL, dev_nr, NULL, "%s", "led_onoff_ru")))
    {
        pr_err("signalru: device_create failed ...\n");
        goto free_class;
    }

    // Init gpios
    if(config_gpios() != 0)
    {
        goto free_device;
    }

    // Init wait queue
    init_waitqueue_head(&freq_wq);

    // Create and start kernel thread
    blink_thread = kthread_run(blink_thread_func, NULL, "blink_thread");
    if(blink_thread)
    {
        pr_info("signalru: created kernel thread ...\n");
    }
    else
    {
        pr_err("signalru: failed to create kernel thread ...\n");
        goto free_class;
    }

    pr_info("signalru: sig_driver_init finished ...\n");
    return 0;

free_device:
    device_destroy(dev_class, dev_nr);
free_class:
    class_destroy(dev_class);
free_cdev:
    unregister_chrdev_region(dev_nr, 1);
    cdev_del(&sig_cdev);

    return -EIO;
}

static void __exit sig_driver_exit(void)
{
    pr_info("signalru: sig_driver_exit start ...\n");

    // Kill thread
    kthread_stop(blink_thread);

    // Free gpios
    free_gpios();

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
