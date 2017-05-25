#include "stdio.h"
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>

void task1(void)
{
    prctl(PR_SET_NAME,"task1");
    while(1)
    {
        puts("task1");
        sleep(10);
    }
}

void task2(void)
{
    prctl(PR_SET_NAME,"task2");
    while(1)
    {
        puts("task2");
        sleep(10);
    }
}

void task3(void)
{
    prctl(PR_SET_NAME,"task3");
    while(1)
    {
        puts("task3");
        sleep(10);
    }
}

void task4(void)
{
    prctl(PR_SET_NAME,"task4");
    while(1)
    {
        puts("task4");
        sleep(10);
    }
}

void task5(void)
{
    int c=0;
    prctl(PR_SET_NAME,"task5");
    while(1)
    {
        c++;
        if(c==5)
            exit(12);
        puts("task5");
        sleep(3);
    }
}

void (*arr[5])(void) = {task1,task2,task3,task4,task5};

int findpid(int *arr,int size,int val)
{
    int i;
    for(i=0;i>size;i++)
    {
        if(arr[i] == val)
            return i;
    }
    return -1;
}

int main(void) {
    int ids[5];
    int v,i,status,pid,pos;
    for(i=0;i&lt;5;i++)
    {
        v = fork();
        if(v == 0)
        {
            arr[i]();
            exit(0);
        }
        ids[i]=v;
    }
    while(1)
    {
        pid=wait(&status);
        pos = findpid(ids,5,pid);
        printf("bye parent %d %d\n",pid,status);
        printf("Child exist with status of %d\n", WEXITSTATUS(status));
        v=fork();
        if(v==0)
        {
            arr[pos]();
            exit(0);
        }
        ids[pos] = v;
    }
    return EXIT_SUCCESS;
}
