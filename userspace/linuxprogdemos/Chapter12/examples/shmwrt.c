/*
   shmwrt.c  Chapter 12

*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>  
#include <sys/stat.h>
#include <stdlib.h>

#define SIZE 1024

int main()
{
  int shmid, i;
  int *shmptr;

  key_t key=ftok(getenv("HOME"),0);
  
  shmid = shmget(key, SIZE*sizeof(int),IPC_CREAT|S_IRWXU);
  if (shmid == -1)
  { 
     perror("shmget failed"); 
     exit(1); 
  }

	/* Attach it to the address space; attached address is
      returned as a pointer */
  shmptr = (int *) shmat(shmid, 0, 0);
  if (shmptr == (int *)-1) 
  { 
     perror("Can't attach"); 
     exit(1); 
  }

	/* Treat shared segment as 1024 element array */
  for (i = 0; i < SIZE; i++) 
      shmptr[i] = i*2;

}
