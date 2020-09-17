#include <stdlib.h>
#include <fcntl.h>

#define BUFFER 1024

static char kernal_buffer[BUFFER];


int main()
{

	int my_device = open("/dev/simple_character_device", O_RDWR);

	printf("Now testing simple_character_device\n");

  	printf("First Testing Read\n");

  	read(my_device, kernal_buffer, BUFFER);

  	printf("Read output: %s\n", kernal_buffer);

  	return 0;

}
