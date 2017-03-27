#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Display values from the environment.
 * If argument(s) supplied, treat these as environment
 * variable names, print their values (if set).
 * If no arguments supplied, print all environment 
 * variables and their values.
 */

extern char **environ;

int main (int argc, char *argv[])
{
    if ( argc == 1 ) {
	/* 
	 * Print complete environment
	 */
        char **ep = environ;

        while ( *ep ) {
	    printf("%s\n", *ep);
	    ep++;
        }

    } else {
	/* 
	 * Print env variables defined in argv vector 
	 */

        int i;
        char *ev;

        for (i=1; i<argc; i++) {
	    ev = getenv(argv[i]);
            if (ev != NULL)
	        printf("%s=%s\n", argv[i], ev);
	    else
	        printf("Environment variable %s is not defined\n", argv[i]);
        }
    }
}
