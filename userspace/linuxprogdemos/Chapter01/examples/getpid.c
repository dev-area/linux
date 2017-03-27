#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid;

    if ((pid=getpid())  == -1)
    {
	perror ("Failed to get pid");
	exit (1);	
    }

    printf ("Your process id is %d\n", pid);

    return 0;
}
