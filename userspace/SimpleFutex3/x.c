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
int m2=1;

void *threadfn11(void *p)
{
	puts("thread11 start");
	futex(&m1,FUTEX_WAIT,m1,NULL,NULL,0);
	puts("thread11 after lock");
	return NULL;
}
void *threadfn12(void *p)
{
	puts("thread12 start");
	futex(&m2,FUTEX_WAIT,m2,NULL,NULL,0);
	puts("thread12 after lock");
	return NULL;
}

void *threadfn2(void *p)
{
	sleep(2);
	puts("thread2 before lock");
	futex(&m1,FUTEX_WAKE_OP,1,NULL,&m2,FUTEX_OP(FUTEX_OP_ADD,2,FUTEX_OP_CMP_EQ,1));
	printf("m1=%d m2=%d\n",m1,m2);
	return NULL;
}


int main()
{
	pthread_t t1,t2;
	pthread_create(&t1,NULL,threadfn11,NULL);
	pthread_create(&t1,NULL,threadfn12,NULL);
	pthread_create(&t2,NULL,threadfn2,NULL);
	sleep(15);
//	puts("end test");
	return 0;
}
