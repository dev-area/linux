/*
   forkexec.c  Chapter 7

*/

#include <sys/types.h>
#include <stdio.h>     /* for NULL */

int main ()
{
  pid_t pid;

  switch (pid=fork()) 
  {
    case -1: perror("fork failed"); 
             exit(1);

    case  0: execlp ("ls", "ls", "-l", NULL);
             perror ("child exec failed"); 
             exit(2);

    default: execlp ( "ps", "ps", "-f", NULL);
             perror ("exec failed"); 
             exit(2);
  }  

}
