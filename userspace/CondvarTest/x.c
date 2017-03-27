#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t mx;
pthread_cond_t cv;
volatile int flag = 0;

void *threadfn1(void *p)
{
	puts("thread1 start");
	pthread_mutex_lock(&mx);
	puts("thread1 after lock");
	while(flag==0)
	{
		pthread_cond_wait(&cv,&mx);
	}
	flag = 0;
	puts("thread 1 after wait");
	pthread_mutex_unlock(&mx);
	
	return NULL;
}

void *threadfn3(void *p)
{
	puts("thread3 start");
	pthread_mutex_lock(&mx);
	puts("thread3 after lock");
	while(flag==0)
	{
		pthread_cond_wait(&cv,&mx);
	}
	flag = 0;
	puts("thread 3 after wait");
	pthread_mutex_unlock(&mx);
	
	return NULL;
}

void *threadfn2(void *p)
{
	puts("thread2 before lock");
	pthread_mutex_lock(&mx);
	puts("thread2 after lock");
	flag=1;
	pthread_cond_signal(&cv);
	//pthread_cond_broadcast(&cv);
	puts("thread2 after signal");
	
	pthread_mutex_unlock(&mx);
	return NULL;
}


int main()
{
	pthread_t t1,t2,t3;
	pthread_mutex_init(&mx,NULL);
	// uncomment for test
	//pthread_cond_signal(&cv);
	//flag = 1;
	pthread_create(&t1,NULL,threadfn1,NULL);
	pthread_create(&t3,NULL,threadfn3,NULL);
	sleep(1);
	pthread_create(&t2,NULL,threadfn2,NULL);
	sleep(10);
	puts("end test");
	return 0;
}
