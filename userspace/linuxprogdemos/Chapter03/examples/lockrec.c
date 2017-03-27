/*
   lockrec.c  Chapter 3

   Partial function only ...
*/

#define TRUE  1
#define FALSE 0

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int lockrec(int fd, off_t recno, off_t recsize);

int main (int argc, char *argv[])
{
   /* Dummy */

}

int lockrec(int fd, off_t recno, off_t recsize)
{
  /* Argument details for fcntl() */
  struct flock wlock = {0};
  struct flock qlock = {0};
  
  wlock.l_type   = F_WRLCK;
  wlock.l_start  = recno*recsize;
  wlock.l_whence = SEEK_SET;
  wlock.l_len    = recsize;

  qlock.l_type   = F_WRLCK;
  qlock.l_start  = recno*recsize;
  qlock.l_whence = SEEK_SET;
  qlock.l_len    = recsize;

  if (fcntl(fd, F_SETLK, &wlock) < 0) 
  {
    /* Record is locked, but by whom? */
    if (fcntl(fd, F_GETLK, &qlock) < 0)
    { 
       perror("get lock failed"); 
       return FALSE; 
    }

    printf ("Record %d locked by process %d\n", recno, qlock.l_pid);
    printf ("waiting...");

    /* Having printed the message, repeat lock operation, this time
       block if the lock is not free */
    if (fcntl(fd,F_SETLKW,&wlock) < 0)
    { 
       perror("set lock failed"); 
       return FALSE; 
    }

    printf (" record locked\n");

  } 

  return TRUE;

}
