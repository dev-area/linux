#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <unistd.h>
#include <stropts.h>


int main(void)
{
	unsigned int *p;
	int i,memfd;

	memfd = open("/dev/fb0", O_RDWR);

	p = mmap(0, 320*200*4, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, 0x1000);

	puts("start");
	sleep(3);
	for(i=0	;i< 64000;i++)
		p[i]=0x11223344;


	close (memfd);
	return 0;
}
