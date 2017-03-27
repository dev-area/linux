#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#define __USE_GNU
#include <sys/ucontext.h>

void setHandler(void (*handler)(int,siginfo_t *,void *))
{
	struct sigaction action;
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = handler;

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

}


void catchit(int signo, siginfo_t *info, void *extra)
{
	ucontext_t *p=(ucontext_t *)extra;
	int x;
	printf("Signal %d received from parent\n", signo);
	x= p->uc_mcontext.gregs[REG_EIP];
	printf("address = %x\n",x);
	printf("siginfo address=%x\n",info->si_addr);
	setHandler(SIG_DFL);
}



void *threadfn1(void *p)
{
	int x=10;
	while(x--)
	{
		puts("thread1");
		sleep(1);
	}
	return NULL;
}
void gn(int g)
{
	int *p=NULL;
	printf("num=%d\n",g);
	*p=100;
}

void fn()
{
	int g=90;
	g++;
	gn(g);
}

void *threadfn2(void *p)
{
	sleep(5);
	puts("thread2 start");
	fn();
	return NULL;
}

int main()
{
	pthread_t t1,t2;
	int y=90;

	setHandler(catchit);
	y=80;
	pthread_create(&t1,NULL,threadfn1,NULL);
	pthread_create(&t2,NULL,threadfn2,NULL);
	sleep(10);
	puts("end test");
	pthread_exit(NULL);
	return 0;
}
