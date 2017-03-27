/* fork and signal */
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/types.h>


void sig_child(int sig)
{
	pid_t pid;
	int stat;
	puts("signal");
	while((pid=waitpid(-1,&stat,WNOHANG)) > 0)
		printf("child %d terminated\n",pid);
	return;
}

void sig_key(int sig)
{
	puts("ctrl + C");
}

int main()
{
	signal(SIGINT,sig_key);
	signal(SIGCHLD,sig_child);
	if(fork())
	{
		puts("hello world!!!");
		sleep(5);
	}
	else
	{
		execve("/bin/ls",NULL,NULL);
		puts("error in execve");
	}
	return 1;
}
