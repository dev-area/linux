#include<stdio.h>
#include<unistd.h>
#include<pthread.h>


void *threadfn1(void *p)
{
	puts("thread1");
	return NULL;
}

void *threadfn2(void *p)
{
	puts("thread2");
	return NULL;
}


int main()
{
	pthread_t t1,t2,t3;
	
	pthread_attr_t attr;
	
	struct sched_param param;
	  
	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_RR);
	
	param.sched_priority = 50;
	pthread_attr_setschedparam(&attr, &param);
	
	pthread_attr_setstacksize(&attr,20000);
	
	pthread_create(&t1,&attr,threadfn1,NULL);
	
	param.sched_priority = 40;
	pthread_attr_setschedparam(&attr, &param);
	
	pthread_create(&t2,&attr,threadfn2,NULL);
	sleep(10);
	puts("end test");
	return 0;
}




