#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * To show the use of getopt().
 * 
 * The program recognises the following options:
 *    p: print out the process id
 *    u: print out the user id
 * print out any remaining command line parameters
 */

int main (int argc, char **argv)
{
    int c;
    int i;
    int err = 0;

    /* use getopt to process option list */
    /* make sure you suppress getopts error processing */

    opterr = 0;

    while ( (c=getopt(argc, argv, "pu")) != EOF ) {
        /* 
	 * Switch on char in c for required output
	 * output an error message for invalid options 
	 */

        switch (c) 
	{
	    case 'p': 
                printf("Pid = %d\n", getpid());     
                break;
	    case 'u': 
                printf("Uid = %d\n", getuid());
                break;
	    case '?': 
	       fprintf (stderr, "Unrecognised option %c\n", optopt);
	       err = 1;
        }
    }

    for (i=optind; i<argc; i++)
        printf ("Arg %d: %s\n", i-optind+1, argv[i]);

    return err;
}

