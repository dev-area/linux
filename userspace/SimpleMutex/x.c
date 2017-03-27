#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t mx;

void *threadfn1(void *p)
{
	puts("thread1 start");
	pthread_mutex_lock(&mx);
	puts("thread1 after lock");
	sleep(5);
	pthread_mutex_unlock(&mx);
	puts("thread1 after unlock");
	
	return NULL;
}

void *threadfn2(void *p)
{
	puts("thread2 before lock");
	pthread_mutex_lock(&mx);
	puts("thread2 after lock");
	
	pthread_mutex_unlock(&mx);
	return NULL;
}


int main()
{
	pthread_t t1,t2;
	pthread_mutex_init(&mx,NULL);
	pthread_create(&t1,NULL,threadfn1,NULL);
	pthread_create(&t2,NULL,threadfn2,NULL);
	sleep(10);
	puts("end test");
	return 0;
}
