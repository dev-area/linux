#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


int main()
{
	int fd;
	char buf[1000];

	fd=open("/proc/driver/my_proc_file",O_RDONLY);
	
	read(fd,buf,1000);
	puts(buf);
	return 1;
}
