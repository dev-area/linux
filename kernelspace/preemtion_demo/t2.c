#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t tid[2];
int pid[2] = { 1, 2 };
int prio[2] = {50,52};
volatile int count0, count1;

void* doSomeThing(void *arg)
{
    unsigned long i = 0;

    if( *((int *)arg) == 1 )
    {
        printf("\n First thread processing\n");
    }
    else
    {
        printf("\n Second thread processing\n");
    }

    while(1)
    {
        switch( *((int *)arg) )
        {
            case 1:
                ++count0;
                break;
            case 2:
                ++count1;
                break;
            default:
                break;
        }
    }
    return NULL;
}

int main(void)
{
    int i = 0;
    int err;
    struct sched_param param;
    pthread_attr_t attr;
    while(i < 2)
    {
        param.sched_priority = prio[i];
        pthread_attr_init(&attr);
        pthread_attr_setschedpolicy(&attr,SCHED_RR);
        pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedparam(&attr, &param);
        err = pthread_create(&(tid[i]), &attr, doSomeThing, &pid[ i ]);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\n Thread created successfully\n");

        i++;
    }
    for(;;)
    {
        printf("%d %d \n", count0, count1);
        sleep(1);
    }
    pthread_join(tid[0], 0);
    pthread_join(tid[1], 0);
    return 0;
}
