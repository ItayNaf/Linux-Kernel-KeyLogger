#include "unname.h"

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Itay Nafrin");
MODULE_DESCRIPTION("Registers a device number. Auto create /dev file. Add information from the keys pressed to the file.");





/**
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
        printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", MAJOR(dev_nr), MINOR(dev_nr));

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
        cdev_del(&my_device);
        device_destroy(my_class, dev_nr);
        class_destroy(my_class);
        unregister_chrdev_region(dev_nr, 1);
        printk("Goodbye, Kernel\n");
}

module_init(keylogger_init);
module_exit(keylogger_exit);
