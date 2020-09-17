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

loff_t seek_off = 20;

printf("Testing llseek with SEEK_SET\n");
loff_t set = llseek(my_device, seek_off, SEEK_SET);
printf("llseek output: %s\n", set);

printf("Testing llseek with SEEK_CUR\n");
loff_t cur = llseek(my_device, seek_off, SEEK_CUR);
printf("llseek output: %s\n", cur);

close(my_device);

return 0;
}
