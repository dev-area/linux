/*
   fifoclt.c  Chapter 8

*/

#include <stdio.h>
#include <fcntl.h>

#define MAXBUF 2048

int main ()
{  
  int  fd, n;
  char buffer[MAXBUF];
  
  /* Assume named pipe has been created already */

  if ((fd=open("fifo",O_WRONLY,0)) == -1)
  {  /* Block here until writer opens the named pipe */
     perror ("fifo does not exist"); 
     exit (1); 
  }

  sprintf (buffer, "Message from client %d", getpid());

  /* Just write the message to the descriptor, it will appear
     on the read descriptor in the reader process */
  n = write(fd, buffer, strlen(buffer));
  if (n < 0)
  { 
     perror("write to FIFO failed"); 
     exit(1); 
  }

  close (fd);

}
