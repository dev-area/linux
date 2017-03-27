#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t mx;

pthread_cond_t cv;

volatile int flag = 0;

void *threadfn1(void *p)
{
	puts("thread1 start");
	return 100;
}

void *threadfn2(void *p)
{
	puts("thread2 start");
	return NULL;
}

int main()
{
	int x=0;
	pthread_t t1,t2;
	pthread_create(&t1,NULL,threadfn1,NULL);
	pthread_create(&t2,NULL,threadfn2,NULL);
//	sleep(3);
//	pthread_join(t1,&x);
	printf("end test %d\n",x);
	pthread_exit(NULL);
	return 0;
}
