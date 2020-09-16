

#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include <errno.h>

#define buffer_length 1024
static char kernal_buffer[buffer_length]

int open_count = 0;
int release_count = 0;


ssize_t my_read(struct file *my_file, char *buffer, size_t count, loff_t *offset)
{

	int to_read = count

	//copy_to_user
	//The function returns zero on success or non-zero to indicate the number of bytes that weren'ttransferred.
	int ctu = copy_to_user(buffer, kernal_buffer, to_read);

	//error if bytes aren't transferred?
	//if(ctu != 0) return -1;

	//number of
	int result = to_read - ctu;

	printk(KERN_ALERT "Device has read %d bytes\n",result);
	return result;

};

ssize_t my_write(struct file *my_file, char *buffer, size_t size, loff_t *offset)
{

};

int my_open(void)
{
	printk(KERN_ALERT "Opening simple_character_device\n");
	open_count++
	printk(KERN_ALERT "Device has been opened %d times\n",open_count);

	return 0;
};

int my_release(void)
{
	printk(KERN_ALERT "Releasing simple_character_device\n");
	release_count++
	printk(KERN_ALERT "Device has been released %d times\n",release_count);

	return 0;
};

//file operations

static struct file_operations my_device_operations = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,

};

int my_driver_init(void)
{
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	register_chrdev(240,"simple_character_device", &file_operations)
	return 0;
};


int my_driver_exit(void)
{
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	unregister_chrdev(240,"simple_character_device")
	return 0;
};


module_init(my_driver_init);
module_exit(my_driver_exit);
