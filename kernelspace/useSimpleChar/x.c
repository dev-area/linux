#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <poll.h>

int main()
{
	int fd;
	int m=100;
	int *map;
	struct pollfd fds[2];
	fd=open("/dev/simpdev",O_RDWR);
	fds[0].fd = fd;
	
	poll(fds,1,0);
	puts("after polling");
	
	
	
	
	return 1;
}
