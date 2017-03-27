#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

/*
 * Display the values of various system limits.
 * 
 * The program recognises the following options:
 *    f <dir> : print out max pathname and filename lengths
 *              for the filesystem containing <dir>
 *    s:        print out system page size, max number of 
 *              child processes and max size of argv/env arrays
 *
 */

int main (int argc, char **argv)
{
    int c;
    int i;
    int err = 0;
    void show_sys_limit(int), show_fs_limit(char *, int);

    /* 
     * Use getopt to process option list.
     * Suppress getopts error processing.
     */

    opterr = 0;

    while ( (c=getopt(argc, argv, "sf:")) != EOF ) {

        switch (c) {
	case 's': 
	    printf("System limits\n");
	    printf("Max argv:  ");
	    show_sys_limit(_SC_ARG_MAX);
	    printf("Max files: ");
	    show_sys_limit(_SC_OPEN_MAX); 
	    printf("\n");
	    break;
	case 'f': 
	    printf("Filesystem limits for %s\n", optarg);
	    printf("Filename length: ");
	    show_fs_limit(optarg, _PC_NAME_MAX);
	    printf("Pathname length: ");
	    show_fs_limit(optarg, _PC_PATH_MAX);
	    printf("\n");
	    break;
	case '?': 
	    fprintf (stderr, "Unrecognised option %c\n", optopt);
	    err = 1;
        }
    }
    
    return err;
}


/*
 * Determine the value of the system limit specified in the argument <lim>
 * Print an appropriate message to cover all three outcomes:
 *   - limit not supported on the system (sysconf() returns -1 but errno is not set)
 *   - some error occurred (sysconf() returns -1 and errno set)
 *   - the value is returned
 */
void show_sys_limit (int lim)
{
    int limit;
    
    errno = 0;

    if ( (limit = sysconf(lim)) < 0 ) {
	if ( errno == 0 ) 
	    printf("Facility not supported on system\n");
	else 
	    printf("Can't determine limit: %s\n", strerror(errno));
    } else
	printf("Current value: %d\n", limit);

    return;
}


/*
 * Determine the value of the filesystem limit specified in the argument <lim>, as
 * it relates to the filesystem containing the directory specified in <dir>.
 * Print an appropriate message to cover all three outcomes:
 *   - limit not supported on the system (sysconf() returns -1 but errno is not set)
 *   - some error occurred (sysconf() returns -1 and errno set)
 *   - the value is returned
 */
void show_fs_limit (char *dir, int lim)
{
    int limit;
    
    errno = 0;

    if ( (limit = pathconf(dir, lim)) < 0 ) {
	if ( errno == 0 ) 
	    printf("Facility not supported on system\n");
	else 
	    printf("Can't determine limit: %s\n", strerror(errno));
    } else
	printf("Current value: %d\n", limit);

    return;
}
