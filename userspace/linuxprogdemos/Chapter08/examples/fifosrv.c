/*
   fifosrv.c  Chapter 8

*/

#include <stdio.h>
#include <fcntl.h>

#define MAXBUF 2048

int main ()
{  
  int  fd, n;
  char buffer[MAXBUF];

  /* Assume someone has already created the named pipe */

  if ((fd=open("fifo",O_RDONLY,0)) == -1)
  {  /* Block until here until a writer opens the named pipe */
     perror ("fifo does not exist"); 
     exit (1); 
  }

  /* Just read and print everything set to us */
  while ((n=read(fd,buffer,MAXBUF-1)) != 0)
  {
    if (n == -1)
    {
       perror ("FIFO read failed"); 
       exit(1); 
    }

    buffer[n] = '\0';
    printf ("Message: %s\n", buffer);
  }

  close (fd);

}
