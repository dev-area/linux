#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <sys/ucontext.h>
#include <ucontext.h>



void catchit(int signo, siginfo_t *info, void *extra) 
{
	ucontext_t *p=(ucontext_t *)extra;
	// dump some registers value
}


int main()
{
	int x=9,y=0,z;
	int *p=NULL;

	// TODO: install fault handler for SIGSEGV, SIGFPE, SIGILL, SIGBUS	
	p=90;
	
	// uncomment one of the following lines to test your work
	//*p=100;
	//z=x/y;
	printf("%d\n",z);
	return 0;
}


