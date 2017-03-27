#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


int main(int argc,char *argv[])
{
	char buf[11];
	int pipedesc;
	puts("child started");
	pipedesc = atoi(argv[0]);
	while(1)
	{
		read(pipedesc,buf,10);
		puts(buf);
	}
	return 0;
}
