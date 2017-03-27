#include<stdio.h>
#include<unistd.h>
#include<pthread.h>


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
	pthread_create(&t1,NULL,threadfn1,NULL);
	pthread_create(&t2,NULL,threadfn2,NULL);
	sleep(10);
	puts("end test");
	pthread_exit(NULL);
	return 0;
}
