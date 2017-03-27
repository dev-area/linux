#include <stdlib.h>

int main ()
{
	int status = system( "ls -al > foo");

	if ( status == -1 ) 
		perror("Call to system failed");
	else
		printf("Command exit status was %d\n", status);
}
