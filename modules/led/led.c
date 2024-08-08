#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>

static struct gpio_desc* gpio_desc = NULL;
static dev_t             led_dev_number;
static struct cdev       driver_object;
static struct class*     led_class;
static struct device*    led_dev;

static int config_gpios(void)
{
    pr_info("led.c: config_gpios called\n");

    struct device_node* nodeptr = led_dev->of_node;
    nodeptr                     = of_find_node_by_name(nodeptr, "custom_gpio");
    led_dev->of_node            = nodeptr;

    gpio_desc = gpiod_get(led_dev, "my", GPIOD_OUT_LOW);
    if(IS_ERR(gpio_desc))
    {
        pr_err("led.c: gpiod_get failed\n");
        return -EIO;
    }

    pr_info("led.c: all gpios reserved and configured\n");
    return 0;
}

static int free_gpios(void)
{
    pr_info("led.c: free_gpios called\n");

    if(gpio_desc)
    {
        gpiod_put(gpio_desc);
    }

    return 0;
}

static ssize_t driver_write(struct file* instanz, const char __user* user, size_t max_bytes_to_write, loff_t* offset)
{
    pr_info("led.c: driver_write called\n");

    unsigned long bytes_to_copy = 1, bytes_not_copied = 0, bytes_copied = 0;
    int value = 0;

    bytes_not_copied            = copy_from_user(&value, user, bytes_to_copy);
    bytes_copied                = bytes_to_copy - bytes_not_copied;
    *offset                    += bytes_copied;

    pr_info("led.c: bytes_copied (%ld)\n", bytes_copied);

    if(value == 0x0 || value == 0x1)
    {
        gpiod_set_value(gpio_desc, value);
        pr_info("led.c: value set (%d)\n", value);
    }
    else
    {
        pr_info("led.c: wrong input (%d)\n", value);
    }

    return bytes_copied;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = driver_write,
};

static int __init mod_init(void)
{
    pr_info("led.c: mod_init called\n");

    if(alloc_chrdev_region(&led_dev_number, 0, 1, "led") < 0)
    {
        pr_err("led.c: alloc_chrdev_region failed\n");
        return -EIO;
    }

    cdev_init(&driver_object, &fops);
    driver_object.owner = THIS_MODULE;

    if(cdev_add(&driver_object, led_dev_number, 1))
    {
        goto free_device_number;
    }

    led_class = class_create("led");
    if(IS_ERR(led_class))
    {
        pr_err("led.c: no udev support\n");
        goto free_cdev;
    }

    led_dev = device_create(led_class, NULL, led_dev_number, NULL, "%s", "led");
    if(IS_ERR(led_dev))
    {
        pr_err("led.c: device_create failed\n");
        goto free_class;
    }

    if(config_gpios())
    {
        goto free_device;
    }

    pr_info("led.c: mod_init success\n");
    return 0;

free_device:
    device_destroy(led_class, led_dev_number);
free_class:
    class_destroy(led_class);
free_cdev:
    kobject_put(&driver_object.kobj);
free_device_number:
    unregister_chrdev_region(led_dev_number, 1);

    return -EIO;
}

static void __exit mod_exit(void)
{
    pr_info("led.c: mod_exit called\n");

    free_gpios();

    /* Loeschen des Sysfs-Eintrags und damit der Geraetedatei */
    device_destroy(led_class, led_dev_number);
    class_destroy(led_class);

    /* Abmelden des Treibers */
    cdev_del(&driver_object);
    unregister_chrdev_region(led_dev_number, 1);

    return;
}

module_init(mod_init);
module_exit(mod_exit);

/* Metainformation */
MODULE_AUTHOR("Tim Krueger");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A driver to toggle GPIO Pin 23.");
