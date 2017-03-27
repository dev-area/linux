/*
 * writeDataTask.c
 *
 *  Created on: May 15, 2010
 *      Author: root
 */
#include<unistd.h>
#include<stdio.h>
#include <sys/prctl.h>

void writeDataTask()
{
	puts("writeDataTask started");
	prctl(PR_SET_NAME,"WriteData");
	sleep(5);
	while(1);

}
