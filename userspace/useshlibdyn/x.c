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
	int (*fptr)(int,int);
	void *libh=dlopen("libshlibdemo.so",RTLD_LAZY);
	fptr=dlsym(libh,"add");
	printf("sum = %d\n",fptr(20,10));
	return 0;
}





