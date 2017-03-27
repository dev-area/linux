/*
 * x.c
 *
 *  Created on: Jun 17, 2009
 *      Author: root
 */

#include<stdio.h>
#include<dlfcn.h>
int add(int,int);

int main()
{
	printf("sum = %d\n",add(20,10));
	sleep(30);
	return 0;
}
