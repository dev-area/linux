/*
   shmrdr.c  Chapter 12

*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>  
#include <stdlib.h>

#define SIZE 1024

int main()
{
  int shmid, i;
  int *shmptr;

  key_t key=ftok(getenv("HOME"),0);
  
  /* Assume writer process has created the segment already */
  shmid = shmget(key, SIZE*sizeof(int), 0);
  if (shmid == -1)
  { 
     perror("shmget failed"); 
     exit(1); 
  }

	/* Attach the segment as an array of integers */
  shmptr = (int *) shmat(shmid, 0, SHM_RDONLY);
  if (shmptr == (int *)-1) 
  { 
     perror("Can't attach"); 
     exit(1); 
  }

	/* Now scan through, reading all the values */
  for (i = 0; i < SIZE; i++) 
      printf ("%d:%d\n", i, shmptr[i]);

}
