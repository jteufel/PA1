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

char to_write[10] = {'h','e','l','l','o','w','o','r','l','d'};
printf("Write %s to device: %s\n", to_write);
write(my_device, to_write, 10);

printf("Read again\n");
read(my_device, kernal_buffer, BUFFER);
printf("Read output: %s\n", kernal_buffer);

printf("Testing llseek with SEEK_SET\n");
int set = llseek(my_device, 20, SEEK_SET);
printf("llseek output: %s\n", set);

close(my_device);

return 0;
}
