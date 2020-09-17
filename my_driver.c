#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/unistd.h>

#define BUFFER 1024
#define MAJOR 240
static char kernal_buffer[BUFFER];

int open_count = 0;
loff_t file_pointer_location;


ssize_t my_read(struct file *my_file, char *buffer, size_t count, loff_t *offset)
{

	int to_read;
	int ctu;
	int result;

	if (BUFFER > count)
			to_read = count;
	else
			to_read = BUFFER;

	//copy_to_user
	//The function returns zero on success or non-zero to indicate the number of bytes that weren't transferred.
	ctu = copy_to_user(buffer, kernal_buffer, to_read);

	result = to_read - ctu;

	printk(KERN_ALERT "Device has read %d bytes\n",result);

	return result;

};

ssize_t my_write(struct file *my_file, const char *buffer, size_t count, loff_t *offset)
{

	int to_write;
	int cfu;
	int result;

	if (BUFFER > count)
			to_write = count;
	else
			to_write = BUFFER;

	//copy_from_user
	//The function returns zero on success or non-zero to indicate the number of bytes that weren'ttransferred.
	cfu = copy_from_user(kernal_buffer, buffer, to_write);

	result = to_write - cfu;

	printk(KERN_ALERT "Device has written %d bytes\n",result);

	return result;

};

loff_t my_llseek(struct file *my_file, loff_t offset, int whence)
{
	
	printk(KERN_ALERT "In llseek\n");
	switch (whence) {
		case SEEK_SET:
			printk(KERN_ALERT "SEEK_SET\n");
			file_pointer_location = offset;
		case SEEK_CUR:
			printk(KERN_ALERT "SEEK_CUR\n");
			file_pointer_location = offset + file_pointer_location;
		case SEEK_END:
			printk(KERN_ALERT "SEEK_END\n");
			file_pointer_location = BUFFER - offset;

	};
	
	printk(KERN_ALERT "llseek returning %d",file_pointer_location);
	return file_pointer_location;
};

int my_open(struct inode *pinode, struct file *my_file)
{

	loff_t starting_loc;
	printk(KERN_ALERT "Opening simple_character_device\n");

	open_count++;
	file_pointer_location = 0;

	printk(KERN_ALERT "Device has been opened %d times\n",open_count);

	return 0;
};

int my_release(struct inode *pinode, struct file *my_file)
{
	//int release_count;
	printk(KERN_ALERT "Releasing simple_character_device\n");

	return 0;
};

//file operations

static struct file_operations my_device_operations = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,
	.llseek = my_llseek,

};

int my_driver_init(void)
{
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	register_chrdev(MAJOR,"simple_character_device", &my_device_operations);
	return 0;
};


void my_driver_exit(void)
{
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	unregister_chrdev(MAJOR,"simple_character_device");
};


module_init(my_driver_init);
module_exit(my_driver_exit);
