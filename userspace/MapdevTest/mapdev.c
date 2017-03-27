#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <unistd.h>
#include <stropts.h>




void closephys (void)
{
}

int main(void)
{
	unsigned int *memregs32;
	int memfd;
	char buf[10000000];
	memfd = open("/dev/mem", O_RDWR);


	memregs32 = mmap(0, 0x100, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, 0x101e8000);

	printf ("RTC value = %04X\n", memregs32[0]);
	sleep(2);
	printf ("RTC value = %04X\n", memregs32[0]);
	
	memregs32[2]=0x1000;
	sleep(2);
	printf ("RTC value = %04X\n", memregs32[0]);
	
	printf("test complete\n");
	close (memfd);
	return 0;
}
