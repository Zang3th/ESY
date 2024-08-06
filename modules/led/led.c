#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio/consumer.h>

static struct gpio_desc* my_gpio_desc = NULL;
static char              input_value[1];
static dev_t             led_dev_number;
static struct cdev*      driver_object;
static struct class*     led_class;
static struct device*    led_dev;

static int driver_open(struct inode* geraete_datei, struct file* instanz)
{
    dev_info(led_dev, "driver_open called\n");
    int success = 0;

    my_gpio_desc = gpio_to_desc(23);
    if(!my_gpio_desc)
    {
        dev_err(led_dev, "gpio_to_desc returned NULL\n");
        success = 1;
    }
    if(IS_ERR(my_gpio_desc))
    {
        dev_err(led_dev, "gpio_to_desc returned error: %ld\n", PTR_ERR(my_gpio_desc));
        success = 1;
    }

    if(success == 0)
    {
        int err = gpiod_direction_output(my_gpio_desc, 1);
        if(err)
        {
            dev_err(led_dev, "gpiod_direction_output: %d\n", err);
            gpiod_put(my_gpio_desc);
            return -EIO;
        }

        dev_info(led_dev, "GPIO 23 successfully acquired and set as output\n");
        return 0;
    }

    // Try other GPIO reserving method
    my_gpio_desc = gpiod_get(NULL, "gpio23", GPIOD_OUT_HIGH);
    if (!my_gpio_desc) {
        dev_err(led_dev, "gpiod_get returned NULL\n");
        return -EIO;
    }
    if (IS_ERR(my_gpio_desc)) {
        dev_err(led_dev, "gpiod_get returned error: %ld\n", PTR_ERR(my_gpio_desc));
        return -EIO;
    }

    // dev_info(led_dev, "GPIO 23 successfully acquired and set as output\n");
    return 0;
}

static int driver_close(struct inode* geraete_datei, struct file* instanz)
{
    dev_info(led_dev, "driver_close called\n");
    gpiod_put(my_gpio_desc);
    return 0;
}

static ssize_t driver_read(struct file* instanz, char __user* user, size_t count, loff_t* offset)
{
    dev_info(led_dev, "driver_read called\n");
    return 0;
}

static ssize_t driver_write(struct file* instanz, const char __user* user, size_t max_bytes_to_write, loff_t* offset)
{
    dev_info(led_dev, "driver_write called\n");

    unsigned long bytes_to_copy = 1, bytes_not_copied, bytes_copied;
    bytes_not_copied            = copy_from_user(input_value, user, bytes_to_copy);
    bytes_copied                = bytes_to_copy - bytes_not_copied;
    *offset                     = *offset + bytes_copied;

    dev_info(led_dev, "bytes_copied: %ld\n", bytes_copied);
    dev_info(led_dev, "user gave: %c\n", input_value[0]);

    int val = 0;
    if(input_value[0] != '0') val = 1;

    dev_info(led_dev, "value: %d\n", val);

    gpiod_set_value(my_gpio_desc, val);

    return bytes_copied;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .read    = driver_read,
    .write   = driver_write,
    .open    = driver_open,
    .release = driver_close,
};

static int __init mod_init(void)
{
    if(alloc_chrdev_region(&led_dev_number, 0, 1, "led") < 0) return -EIO;

    /* Anmeldeobjekt reservieren */
    driver_object = cdev_alloc();
    if(driver_object == NULL) goto free_device_number;
    driver_object->owner = THIS_MODULE;
    driver_object->ops   = &fops;
    if(cdev_add(driver_object, led_dev_number, 1)) goto free_cdev;

    /* Eintrag im Sysfs, damit Udev den Geraetedateieintrag erzeugt. */
    led_class = class_create("led");
    if(IS_ERR(led_class))
    {
        pr_err("led: no udev support\n");
        goto free_cdev;
    }
    led_dev = device_create(led_class, NULL, led_dev_number, NULL, "%s", "led");
    if(IS_ERR(led_dev))
    {
        pr_err("led: device_create failed\n");
        goto free_class;
    }

    printk("led.c: mod_init called\n");

    return 0;

free_class:
    class_destroy(led_class);
free_cdev:
    kobject_put(&driver_object->kobj);
free_device_number:
    unregister_chrdev_region(led_dev_number, 1);

    return -EIO;
}

static void __exit mod_exit(void)
{
    /* Loeschen des Sysfs-Eintrags und damit der Geraetedatei */
    device_destroy(led_class, led_dev_number);
    class_destroy(led_class);

    /* Abmelden des Treibers */
    cdev_del(driver_object);
    unregister_chrdev_region(led_dev_number, 1);

    printk("led.c: mod_exit called\n");

    return;
}

module_init(mod_init);
module_exit(mod_exit);

/* Metainformation */
MODULE_AUTHOR("Tim Krueger");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A driver to toggle GPIO Port 23.");
