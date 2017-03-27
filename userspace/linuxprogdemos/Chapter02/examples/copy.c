/*
   copy.c  Chapter 2
*/

#include <fcntl.h>
#include <stdio.h>

#define MAXBUF 2048

int main (int argc, char *argv[])
{
  int rfd, wfd;
  size_t n;
  char buffer[MAXBUF];

  if (argc != 3) 
  { 
     fprintf(stderr,"Usage: mycopy source dest\n"); 
     exit (1); 
  } 
  
  if ( (rfd = open(argv[1], O_RDONLY, 0)) == -1 )
  { 
     perror (argv[1]); 
     exit (2); 
  } 

  if ( (wfd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666)) == -1 )
  { 
     perror (argv[2]); 
     exit (2); 
  }

  while ((n=read(rfd, buffer, MAXBUF)) > 0)
    if (write(wfd, buffer, n) != n)
    { 
       perror ("Write"); 
       exit(2); 
    }

  if (n == -1)
  { 
     perror("Read"); 
     exit(2); 
  }

  close(rfd);  
  close(wfd);    
  exit(0);

}
