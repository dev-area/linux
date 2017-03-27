#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

/* 
 * Program to emulate functionality of cat, but avoid 
 * dumping directories etc.
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

    for (i=1; i<argc; i++)
    {
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
 * Function to copy from specified file descriptor to stdout
 */
void doFile(int fd)
{
    char buffer[MAXBUF];
    size_t n;
    struct stat sbuf;

    /*
     * Before dumping the fd, check it's not a directory,
     * we shouldn't really allow directories to be shown
     * in this way...
     */
    if ( fstat(fd, &sbuf) < 0 ) {
	perror("stat");
	return;
    }

    if ( S_ISDIR(sbuf.st_mode) ) {
	printf("Can't dump a directory\n");
	return;
    }

    while ((n=read(fd,buffer,MAXBUF)) > 0) {
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


