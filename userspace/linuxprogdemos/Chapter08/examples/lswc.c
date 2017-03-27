/*
   lswc.c  Chapter 8

*/

#include <stdio.h>

int main ()
{
  int fds[2];		/* buffer for pipe file descrp */
  
  if (pipe(fds) == -1)
  { 
     perror("pipe failed"); 
     exit(1); 
  }
  
  switch (fork()) 
  {
    case -1: 
       perror("fork failed");    
       break;
    case  0: 
      close (fds[0]);	/* Close read end of pipe */
      dup2 (fds[1],1);	/* Set write end to be stdout */
      close (fds[1]);	/* Now close original pipe desc */
      execlp ("ls", "ls", NULL);       
      break;
    default: 
      close (fds[1]);	/* Close write end of pipe */
      dup2 (fds[0],0);	/* Set read end to be stdin */
      close (fds[0]);	/* Now close original pipe desc */
      execlp ("wc", "wc", "-l", NULL); 
      break;
  }  

}
