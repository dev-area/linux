/* fork and signal */
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdlib.h>


void sig_abr(int sig)
{
	puts("aborted");

}

int main()
{
	signal(SIGABRT,sig_abr);
	abort();
	puts("after abort");
	return 1;
}
