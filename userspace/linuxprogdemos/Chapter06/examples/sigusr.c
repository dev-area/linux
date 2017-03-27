/*
   sigusr.c  Chapter 6

*/

#include <stdio.h>
#include <signal.h>

void handler (int id);

int main (int argc, char *argv[])
{
  struct sigaction new;

  sigset_t emptymask, procmask;

  printf("%d\n", getpid());
  sigemptyset (&emptymask);

  new.sa_handler = handler;
  new.sa_mask    = emptymask;
  new.sa_flags   = 0;

  sigaction (SIGUSR1, &new, NULL);
  sigaction (SIGUSR2, &new, NULL);
  sigemptyset (&procmask);
  sigaddset (&procmask, SIGINT);

  /* wait for signals */
  for (;;) 
  {
     sigsuspend(&procmask);
     write(1, "..round again..\n", strlen("..round again..\n"));
  } 
  
}  /* main */

/* --------------------------------------------------------------- */

void handler (int id)
{

  if (id == SIGUSR1)
     write( 1, "SIGUSR1\n", strlen("SIGUSR1\n") );

  if (id == SIGUSR2) 
     write( 1, "SIGUSR2\n", strlen("SIGUSR2\n") );

}
