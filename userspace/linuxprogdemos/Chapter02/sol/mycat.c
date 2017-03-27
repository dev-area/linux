#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/* 
 * Program to emulate the functionality of cat 
 */

#define MAXBUF 1024

int main (int argc, char *argv[])
{
    int fd;
    int i;
    void doFile (int);

    /* 
     * If no arguments, copy stdin to stdout.
     */
    if (argc == 1) 
	doFile (0);

    for (i=1; i<argc; i++) {

	/*
	 * Process each argument one at a time, open the file and
	 * then copy its contents to stdout.
	 */
	fd = open(argv[i],O_RDONLY,0);
	if (fd < 0) {
	    perror(argv[i]);
	    exit(1);
	}

	doFile(fd);
	close(fd);
    }

    return 0;

}

/*
 * Copy from specified file descriptor to stdout
 */
void doFile(int fd)
{
    char buffer[MAXBUF];
    size_t n;

    while ( (n=read(fd,buffer,MAXBUF)) > 0 ) {
	if (write(1,buffer,n) < 0) {
	    perror("write to stdout failed");
	    exit (1);
	}
    }

    if (n < 0) {
        perror("read from file failed");
        exit (1);
    }
}


