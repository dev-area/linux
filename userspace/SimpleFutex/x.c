#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<sys/syscall.h>
#include<linux/futex.h>
#include<linux/unistd.h>
#include "syscallsmac.h"



_syscall6(int,futex,void *,addr1,int,op,int,val1,struct timespec*,timeout,void*,addr2,int,val3)


int m1=1;

void *threadfn1(void *p)
{
	puts("thread1 start");
	futex(&m1,FUTEX_WAIT,m1,NULL,NULL,0);
	puts("thread1 after lock");
	sleep(2);

	return NULL;
}

void *threadfn2(void *p)
{
	sleep(2);
	puts("thread2 before lock");
	futex(&m1,FUTEX_WAKE,1,NULL,NULL,0);
	return NULL;
}


int main()
{
	pthread_t t1,t2;
	pthread_create(&t1,NULL,threadfn1,NULL);
	pthread_create(&t2,NULL,threadfn2,NULL);
	sleep(15);
//	puts("end test");
	return 0;
}
