#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define __USE_GNU
#include <sys/ucontext.h>

void catchit(int signo, siginfo_t *info, void *extra) 
{
	ucontext_t *p=(ucontext_t *)extra;
	int x;
	printf("Signal %d received from parent\n", signo);
	x= p->uc_mcontext.gregs[32];
	printf("address = %x\n",x);
	printf("siginfo address=%x\n",info->si_addr);
	exit(0);
}


int main()
{
	int x=9,y=0,z;
	struct sigaction action; 


	action.sa_flags = SA_SIGINFO; 
	action.sa_sigaction = catchit;

	if (sigaction(SIGFPE, &action, NULL) == -1) { 
		perror("sigusr: sigaction");
		_exit(1);
	}
	if (sigaction(SIGSEGV, &action, NULL) == -1) { 
		perror("sigusr: sigaction");
		_exit(1);
	}
	if (sigaction(SIGILL, &action, NULL) == -1) { 
		perror("sigusr: sigaction");
		_exit(1);
	}
	if (sigaction(SIGBUS, &action, NULL) == -1) { 
		perror("sigusr: sigaction");
		_exit(1);
	}
	z=x/y;
	printf("%d\n",z);
	return 0;
}


