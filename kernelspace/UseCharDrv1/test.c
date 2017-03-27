#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SIZE 64

struct mydevicestatus
{
	int r1;
	int r2;
	int r3;
};

int main()
{
	struct mydevicestatus status1={33,44,55};
	int fd;
	int m=100;
	int *map;
	
	fd=open("/dev/fb0",O_RDWR);	
	map = mmap(0, 320*200*4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x10000);
	
	for(m=0;m<60000;m++)
		map[m]=0xaabbccdd;
	
	
	return 1;
}








