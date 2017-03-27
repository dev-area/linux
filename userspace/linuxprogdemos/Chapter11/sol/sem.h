#include <sys/ipc.h>
#include <sys/sem.h>

int sem_open ( key_t );
int sem_create ( key_t, int );
int sem_wait ( int );
int sem_signal ( int );
int sem_setval ( int, int );
int sem_getval ( int );
int sem_remove ( int );

