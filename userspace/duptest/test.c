#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>

int main()
{
	int dp,fd=open("outfile",O_WRONLY | O_CREAT|O_TRUNC,0666);
	
	write(fd,"hello\n",6);
	
	dp=dup(1);
	puts("before");
	dup2(fd,1);
	puts("after");
	dup2(dp,1);
	puts("restore");
	return 0;
}
