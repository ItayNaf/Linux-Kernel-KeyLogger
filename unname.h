#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/types.h>


#define BUFF_SIZE (PAGE_SIZE << 2)

/* data to append to file */
static char msg[BUFF_SIZE];
static size_t buf_index = 0;

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
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
        int to_copy, not_copied, delta;

        /* Get amount of data to copy */
        to_copy = min(count, sizeof(buffer));

        /* Copy data to user */
        not_copied = copy_from_user(buffer, user_buffer, to_copy);
        buffer_pointer = to_copy;

        /* Calculate data */
        delta = to_copy - not_copied;

        return delta;
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
        .read = driver_read
	//.write = driver_write
};
