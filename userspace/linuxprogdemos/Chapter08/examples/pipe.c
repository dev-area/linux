/*
   pipe.c  Chapter 8

*/

#define MAXBUF 2048

#include <stdio.h>

int main (int argc, char *argv[])
{
  int   fds[2];		/* buffer for pipe file descrp */
  char *msg="Test message", buffer[MAXBUF];
  
  if (pipe(fds) == -1)
  { 
     perror("pipe failed"); 
     exit(1); 
  }

  switch (fork()) {
    case -1: 
       perror("fork failed"); 
       break;
    case  0: 
	   /* Child will read from pipe, so close write descriptor */
      close (fds[1]);
      read (fds[0], buffer, MAXBUF);
      printf ("Message from parent: %s\n", buffer);
      break;
    default: 
	   /* Parent will write to pipe, so close read descriptor */
      close (fds[0]);
      if ( argc > 1 )
         write (fds[1], argv[1], strlen(argv[1])+1);
      break;
  }

  return 0; 

}
