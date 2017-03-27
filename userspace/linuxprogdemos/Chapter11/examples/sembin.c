/*
   sembin.c  Chapter 11

*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/stat.h>

struct sembuf lock[1]   = { { 0, -1, SEM_UNDO } };
struct sembuf unlock[1] = { { 0, 1, SEM_UNDO } }; 

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
   /* union semun is defined by including <sys/sem.h> */
#else
   /* according to X/OPEN we have to define it ourselves */
   union semun {
           int val;                    /* value for SETVAL */
           struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
           unsigned short int *array;  /* array for GETALL, SETALL */
           struct seminfo *__buf;      /* buffer for IPC_INFO */
   };
#endif
 
static int GetChoice ();

int main (int argc, char *argv[])
{
  union semun info;
  char choice = 0;
  int semid;

  key_t key=ftok(getenv("HOME"),0);

  if ((semid=semget (key, 1, IPC_CREAT|S_IRWXU)) == -1)
  { 
     perror("semget failed"); 
     exit(1); 
  }

  /* Set initial value to unlocked */
  info.val = 1;
  semctl (semid, 0, SETVAL, info);

  /* Try and make sure first op is lock operation */
  while(choice != 'q') 
  {
      switch (choice=GetChoice()) 
      {
      case 'l': 
         semop (semid, lock, 1);   
         break;
      case 'u': 
         semop (semid, unlock, 1); 
         break;
      }
  }
 
  return 0;

}

static int GetChoice ()
{
   /* Dummy */
   return 'l';
}

