/*
 * uartTask2.c
 *
 *  Created on: May 15, 2010
 *      Author: root
 */
#include<unistd.h>
#include<stdio.h>
#include <sys/prctl.h>


void uartTask2()
{
	puts("uartTask2 started");
	  prctl(PR_SET_NAME,"UART2");
	sleep(5);
	while(1);

}
