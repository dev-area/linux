/*
 * computeTask.c
 *
 *  Created on: May 15, 2010
 *      Author: root
 */
#include<unistd.h>
#include<stdio.h>
#include <sys/prctl.h>

void computeTask()
{
	puts("computeTask started");
	prctl(PR_SET_NAME,"Compute");
	sleep(5);
	while(1);

}
