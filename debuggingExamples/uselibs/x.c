/*
 * x.c
 *
 *  Created on: Jun 17, 2009
 *      Author: root
 */

#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>
int add(int,int);

int main()
{
	int (*fptr)(int,int);
	void *libh=dlopen("libdemo2.so",RTLD_LAZY);
	mtrace();
	if(libh == NULL)
	{
		puts("Error loading library");
		exit(0);
	}
	fptr=dlsym(libh,"_Z3addii");
	if(fptr == NULL)
	{
		puts("Error finding function");
		exit(0);
	}
	printf("sum = %d\n",fptr(20,10));
	return 0;
}





