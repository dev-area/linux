#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  pid_t pid;

  pid = fork();

  switch (pid)
  {
    case -1: perror ("fork failed");
             exit(1);
    case 0:  printf ("Child process %d\n", getpid());
             exit(0);
    default: printf ("Parent started child %d\n", pid);
             break;
  }

  return 0;
}
