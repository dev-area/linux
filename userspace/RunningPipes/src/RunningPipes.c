#include<stdio.h>
#include<unistd.h>

int main()
{
	int arr[2];
	char argv[30];
	pipe(arr);
	if(fork())
	{
		puts("starting parent");
		close(arr[0]);
		sprintf(argv,"%d",arr[1]);
		execlp("./pipeExampleParent",argv,NULL);
	}
	else
	{
		puts("starting child");
		close(arr[1]);
		sprintf(argv,"%d",arr[0]);
		execlp("./pipeExample",argv,NULL);
	}
	return 1;
}
