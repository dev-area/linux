#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "sigreg.h"

static void register_to_sig(void)
{
	int fd = open("/dev/sigreg",O_RDWR);
	ioctl(fd, SIGNAL_REGISTER , 0);
	close(fd);
}

static void set_sig_num(int num)
{
	int fd = open("/dev/sigreg",O_RDWR);
	ioctl(fd, SIGNAL_SETSIG , num);
	close(fd);
}	

static void bc_signal(int val)
{
	int fd = open("/dev/sigreg",O_RDWR);
	ioctl(fd, SIGNAL_BROADCAST , val);
	close(fd);

}

void handler(int signo, siginfo_t *info, void *extra) 
{
       int int_val = info->si_value.sival_int;
       printf("PID=%d Signal %d, value %d  received from parent\n",getpid(), signo, int_val);
}

void set_sig_handler(void)
{
        struct sigaction action;


        action.sa_flags = SA_SIGINFO; 
        action.sa_sigaction = handler;

        if (sigaction(SIGRTMIN + 3, &action, NULL) == -1) { 
            perror("sigusr: sigaction");
            _exit(1);
        }

}

void task1(void)
{
    set_sig_handler();
    register_to_sig();
    prctl(PR_SET_NAME,"task1");
    while(1)
    {
        puts("task1");
        sleep(3);
    }
}

void task2(void)
{
    set_sig_handler();
    register_to_sig();
    prctl(PR_SET_NAME,"task2");
    while(1)
    {
        puts("task2");
        sleep(3);
    }
}

void task3(void)
{
    set_sig_handler();
    register_to_sig();
    prctl(PR_SET_NAME,"task3");
    while(1)
    {
        puts("task3");
        sleep(3);
    }
}

void task4(void)
{
    set_sig_handler();
    register_to_sig();
    prctl(PR_SET_NAME,"task4");
    while(1)
    {
        puts("task4");
        sleep(3);
    }
}




void (*arr[4])(void) = {task1,task2,task3,task4};

int findpid(int *arr,int size,int val)
{
    int i;
    for(i=0;i<size;i++)
    {
        if(arr[i] == val)
            return i;
    }
    return -1;
}

int main(void) {
    int ids[4];
    int v,i,status,pid,pos;
    
    for(i=0;i<4;i++)
    {
        v = fork();
        if(v == 0)
        {
            arr[i]();
            exit(0);
        }
        ids[i]=v;
    }
    set_sig_num(SIGRTMIN+3);
    i=1;
    while(1)
    {
    	sleep(10);
    	bc_signal(i++);
    }
    return EXIT_SUCCESS;
}
