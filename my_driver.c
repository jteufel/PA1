#include<linux/init.h> //from hello world
#include<linux/module.h> //from hello world
#include<linux/fs.h>//
#include<linux/uaccess.h> //for copy to copy_from_user
#include<linux/unistd.h> //needed for read and write calls and ll seek

#define BUFFER 1024
#define MAJOR 240
static char kernal_buffer[BUFFER];

int open_count = 0;
loff_t file_pointer_location;

/*
General notes:

used MakeFile to compile driver code - makes my_driver.ko module file to be inserted

sudo insmod - insert LKM kernal
sudo rmmod - remove LKM from kernal
lsmod - view LKM's in KERN_ALERT

virtual files - a way for the kernal to know which driver to use with which device. Located in the /dev directory.

sudo mknod <location> <type of driver> <major number> <minor number> - used to make a virtual file. Major number should be unique to the device.

register chrdev - called when we inssert the LKM into the kernal, used to tell the kernal which device it will be supporting by passing in the major number

ssize_t is an unsigned integer type used for byte measurments

loff_t is type 'long offset' used for the offset pointer argument in the

*/

//

ssize_t my_read(struct file *my_file, char *buffer, size_t count, loff_t *offset)
{
	/*
	Reads count bytes from the device (kernal space) into the user space buffer.

	Conditional checks to see the count is less then the buffer size.

	Then is calls the copy_to_user which copies data from the kernel into user space.
	It reads data from the kernal_buffer into the user space buffer (*buffer).

	copy_to_user returns the number of bytes that were successfuly transferred,
	but the function is supposed to return the number of byetes that weren't transferred successfuly

	*/
	int to_read;
	int ctu;
	int result;

	if (BUFFER > count)
			to_read = count;
	else
			to_read = BUFFER;

	ctu = copy_to_user(buffer, kernal_buffer, to_read);

	result = to_read - ctu;

	printk(KERN_ALERT "Device has read %d bytes\n",result);

	return result;

};

ssize_t my_write(struct file *my_file, const char *buffer, size_t count, loff_t *offset)
{

	/*
	writes count bytes from user spcae into the device (kernal space buffer).

	Conditional checks to see the count is less then the buffer size.

	Then is calls the copy_from_user which copies data from the kernel into user space.
	It reads data from the kernal_buffer into the user space buffer (*buffer).

	copy_to_user returns the number of bytes that were successfuly transferred,
	but the function is supposed to return the number of byetes that weren't transferred successfuly

	*/

	int to_write;
	int cfu;
	int result;



	if (BUFFER > count)
			to_write = count;
	else
			to_write = BUFFER;

	//copy_from_user
	//The function returns zero on success or non-zero to indicate the number of bytes that weren't transferred.
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
			break;
		case SEEK_CUR:
			printk(KERN_ALERT "SEEK_CUR\n");
			file_pointer_location = offset + file_pointer_location;
			break;
		case SEEK_END:
			printk(KERN_ALERT "SEEK_END\n");
			file_pointer_location = BUFFER - offset;
			break;

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
