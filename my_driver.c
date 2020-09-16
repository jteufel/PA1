

#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

#define buffer_length 1024
static char kernal_buffer[buffer_length];

//int open_count = 0;
//int release_count = 0;


ssize_t my_read(struct file *my_file, char *buffer, size_t count, loff_t *offset)
{

	int to_read;
	int ctu;

	if (buffer_length > count)
			to_read = count;
	elseint ctu
			to_read = count;

	//copy_to_user
	//The function returns zero on success or non-zero to indicate the number of bytes that weren'ttransferred.
	ctu = copy_to_user(buffer, kernal_buffer, to_read);

	//error if bytes aren't transferred?
	//if(ctu != 0) return -1;
	//errno =

	//number of
	int result = to_read - ctu;

	printk(KERN_ALERT "Device has read %d bytes\n",result);
	return result;

};

ssize_t my_write(struct file *my_file, const char *buffer, size_t count, loff_t *offset)
{

	int to_write;
	int cfu;

	if (buffer_length > count)
			to_write = count;
	else
			to_write = buffer_length;

	//copy_from_user
	//The function returns zero on success or non-zero to indicate the number of bytes that weren'ttransferred.
	cfu = copy_from_user(kernal_buffer, buffer, to_write);

	//error if bytes aren't transferred?
	//if(ctu != 0) return -1;
	//errno =

	//number of
	int result = to_write - cfu;

	printk(KERN_ALERT "Device has written %d bytes\n",result);
	return result;

};

int my_open(struct inode *pinode, struct file *my_file)
{	
	int open_count;
	printk(KERN_ALERT "Opening simple_character_device\n");
	open_count++;
	printk(KERN_ALERT "Device has been opened %d times\n",open_count);

	return 0;
};

int my_release(struct inode *pinode, struct file *my_file)
{	
	int release_count;
	printk(KERN_ALERT "Releasing simple_character_device\n");
	release_count++;
	printk(KERN_ALERT "Device has been released %d times\n",release_count);

	return 0;
};

//file operations

static struct file_operations my_device_operations = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write

};

int my_driver_init(void)
{
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	register_chrdev(240,"simple_character_device", &my_device_operations);
	return 0;
};


void my_driver_exit(void)
{
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	unregister_chrdev(240,"simple_character_device");
};


module_init(my_driver_init);
module_exit(my_driver_exit);
