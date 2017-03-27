/*
   system.c  Chapter 7

*/

#include <stdio.h>

int main ()
{
  int status = system("ls -al");
  
  if (status == -1)
    perror("system call failed");
  else
    printf ("Command exit status %d\n", status);

}
