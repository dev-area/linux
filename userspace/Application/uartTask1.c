/*
 * uartTask1.c
 *
 *  Created on: May 15, 2010
 *      Author: root
 */
#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include <sys/prctl.h>

void uartTask1()
{
	int x=0;
	clock_t st,en;
	struct timespec t1;
	t1.tv_nsec = 100000;
	t1.tv_sec = 0;

	puts("uartTask1 started");
	  prctl(PR_SET_NAME,"UART1");
	puts("start ......");
	while(1)
	{
		nanosleep(&t1,NULL);
		x++;
		if(x == 1000)
		{
			puts("exit....");
			exit(0);
		}
	}

}
