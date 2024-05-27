#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio/consumer.h>

static struct gpio_desc* my_gpio_desc = NULL;
static char input_value[1];
static dev_t led_one_dev_number;
static struct cdev *driver_object;
static struct class *led_one_class;
static struct device *led_one_dev;

static int driver_open( struct inode *geraete_datei, struct file *instanz )
{
    int err;
    
    dev_info(led_one_dev, "driver_open called\n");
	int success = 0;

    my_gpio_desc = gpio_to_desc(23);
    if (!my_gpio_desc) {
        dev_err(led_one_dev, "gpio_to_desc returned NULL\n");
		success = 1;
    }
    if (IS_ERR(my_gpio_desc)) {
        dev_err(led_one_dev, "gpio_to_desc returned error: %ld\n", PTR_ERR(my_gpio_desc));
		success = 1;
    }

	if(success == 0)
	{
		err = gpiod_direction_output(my_gpio_desc, 1);
    	if (err) {
        	dev_err(led_one_dev, "gpiod_direction_output: %d\n", err);
        	gpiod_put(my_gpio_desc);
        	return -EIO;
    	}

		dev_info(led_one_dev, "GPIO 23 successfully acquired and set as output\n");
    	return 0;
	}
	
	// Try other GPIO reserving method
	my_gpio_desc = gpiod_get(NULL, "gpio23", GPIOD_OUT_HIGH);
    if (!my_gpio_desc) {
        dev_err(led_one_dev, "gpiod_get returned NULL\n");
        return -EIO;
    }
    if (IS_ERR(my_gpio_desc)) {
        dev_err(led_one_dev, "gpiod_get returned error: %ld\n", PTR_ERR(my_gpio_desc));
        return -EIO;
    }

    dev_info(led_one_dev, "GPIO 23 successfully acquired and set as output\n");
    return 0;
}

static int driver_close( struct inode *geraete_datei, struct file *instanz )
{
	dev_info( led_one_dev, "driver_close called\n" );
    gpiod_put( my_gpio_desc );
	return 0;
}

static ssize_t driver_read( struct file *instanz, char __user *user,
		size_t count, loff_t *offset )
{
    dev_info( led_one_dev, "driver_read called\n" );
	return 0;
}

static ssize_t driver_write( struct file *instanz, const char __user* user,
        size_t max_bytes_to_write, loff_t* offset)
{
    dev_info( led_one_dev, "driver_write called\n" );

    unsigned long bytes_to_copy = 1, bytes_not_copied, bytes_copied;
    bytes_not_copied = copy_from_user(input_value, user, bytes_to_copy);
	bytes_copied = bytes_to_copy - bytes_not_copied;	
	*offset = *offset + bytes_copied;

    dev_info( led_one_dev, "bytes_copied: %ld\n", bytes_copied );
    dev_info( led_one_dev, "user gave: %c\n", input_value[0] );

    int val = 0;
    if(input_value[0] != '0')
        val = 1;

    dev_info( led_one_dev, "value: %d\n", val );

    gpiod_set_value( my_gpio_desc, val );
    
    return bytes_copied;
}

static struct file_operations fops = {
	.owner= THIS_MODULE,
	.read= driver_read,
    .write= driver_write,
	.open= driver_open, 
	.release= driver_close,
};

static int __init mod_init( void )
{
	if (alloc_chrdev_region(&led_one_dev_number,0,1,"led_one")<0)
		return -EIO;

    /* Anmeldeobjekt reservieren */
	driver_object = cdev_alloc();
	if (driver_object==NULL)
		goto free_device_number;
	driver_object->owner = THIS_MODULE;
	driver_object->ops = &fops;
	if (cdev_add(driver_object,led_one_dev_number,1))
		goto free_cdev;

	/* Eintrag im Sysfs, damit Udev den Geraetedateieintrag erzeugt. */
	led_one_class = class_create( "led_one" );
	if (IS_ERR( led_one_class )) {
		pr_err( "led_one: no udev support\n");
		goto free_cdev;
	}    
	led_one_dev = device_create( led_one_class, NULL, led_one_dev_number,
			NULL, "%s", "led_one" );
	if (IS_ERR( led_one_dev )) {
		pr_err( "led_one: device_create failed\n");
		goto free_class;
	}

	return 0;

free_class:
	class_destroy( led_one_class );
free_cdev:
	kobject_put( &driver_object->kobj );
free_device_number:
	unregister_chrdev_region( led_one_dev_number, 1 );

	return -EIO;
}

static void __exit mod_exit( void )
{
	/* Loeschen des Sysfs-Eintrags und damit der Geraetedatei */
	device_destroy( led_one_class, led_one_dev_number );
	class_destroy( led_one_class );

	/* Abmelden des Treibers */
	cdev_del( driver_object );
	unregister_chrdev_region( led_one_dev_number, 1 );
    
	return;
}

module_init( mod_init );
module_exit( mod_exit );

/* Metainformation */
MODULE_AUTHOR("Tim Krueger");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A driver to toggle GPIO Port 23.");
