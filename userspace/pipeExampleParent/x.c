#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


int main(int argc,char *argv[])
{
	char buf[11]="hello";
	int pipedesc,i=0;
	pipedesc = atoi(argv[0]);
	puts("parent started:");
	while(1)
	{
		i++;
		sprintf(buf,"hello:%d",i);
		write(pipedesc,buf,10);
		sleep(2);
	}
	return 0;
}
