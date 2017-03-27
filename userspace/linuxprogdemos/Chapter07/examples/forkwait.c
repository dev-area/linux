/*
   forkwait.c  Chapter 7

*/

#include <sys/types.h>
#include <stdio.h>
#include <wait.h>

int main ()
{
  pid_t pid;
  int status;

  switch (pid=fork()) 
  {
    case -1: perror("parent fork failed"); 
             exit(1);
    case  0: execlp ("ls", "ls", "-al", NULL);
             perror ("child exec failed"); 
             exit(2);
    default: 
      if (wait(&status) > 0 && WIFEXITED(status))
         printf ("Child exited with status %d\n", WEXITSTATUS(status));
  }
}
