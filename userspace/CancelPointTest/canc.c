#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

void clean(void *buf)
{
	puts("cleaning");
	free(buf);
}

void *threadfn1(void *p)
{
	void *pbuf;
	pbuf=malloc(1000);
	if(pbuf!=NULL)
	{
		pthread_cleanup_push(clean,pbuf);
		puts("thread 1 start");
		sleep(7);
		puts("thread 1 end");
		pthread_cleanup_pop(1);
	}
	return NULL;
}

void *threadfn2(void *p)
{
	int x;
	int o_state;
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&o_state);
	
	for(x=0;x<5;x++)
	{
		puts("hello");
	}
	
	pthread_setcancelstate(o_state,&o_state);
	
	return NULL;
}

int main()
{
	pthread_t t1;
	pthread_create(&t1,NULL,threadfn1,NULL);
	sleep(5);
	pthread_cancel(t1);
	printf("end test\n");
	sleep(50);
	return 0;
}
