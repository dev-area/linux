#include<stdio.h>
#include<unistd.h>

int main()
{
	if(fork())
	{
		puts("hello world!!!");
		sleep(5);
	}
	else
	{
		execve("/bin/ls",NULL,NULL);
		puts("error");
	}
	return 1;
}
