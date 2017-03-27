#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXBUF 1024

int main (int argc, char *argv[])
{
    int rfd, wfd, n;
    char buffer[MAXBUF];

    if (argc != 3) 
        { fprintf (stderr, "Usage: mycopy source dest\n"); exit (1); }
    
    rfd = open(argv[1], O_RDONLY, 0);
    if (rfd == -1)
        { perror (argv[1]); exit (2); }
 
    wfd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (wfd == -1)
        { perror (argv[2]); exit (2); }

    while ((n=read(rfd, buffer, MAXBUF)) > 0)
        if (write(wfd, buffer, n) != n)
            { perror ("Write"); exit(2); }
    if (n == -1)
        { perror("Read"); exit(2); }

    close(rfd);  
    close(wfd);
    exit(0);
}
