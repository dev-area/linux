#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>

#define SEM_LOCKED 0
#define SEM_FREE   1

/*
 * Certain Unixes don't define the semun union, for passing
 * arguments to the semctl function. (Linux does.) If it is
 * needed, then uncomment the following definition
 *
 * union semun {
 *     int val;
 *     struct semid_ds *buf;
 *     ushort *array;
 * };
 */

/*
 * Get access to a semaphore specified by <key> that is
 * assumed already to exist.
 */
int sem_open ( key_t key ) 
{
    if ( key >= 0 ) 
	return semget(key, 1, 0);
    else 
	return (-1);
}

/*
 * Get access to a semaphore specified by <key>. If the semaphore
 * does not exist, then create it. Set its value to <init_val>
 */
int sem_create ( key_t key, int init_val )
{
    int s_id;
    union semun info;

    if ( key >= 0 ) {
	s_id = semget(key, 1, (IPC_CREAT|S_IRWXU) );
	if ( s_id >= 0 ) 
	    sem_setval(s_id, init_val);
	return s_id;
    } else 
	return (-1);
}

/*
 * Set the value of the semaphore with id <semid> to <val>
 */
int sem_setval ( int semid, int val )
{
    union semun info;

    info.val = val;
    return semctl(semid, 0, SETVAL, info);
}

/*
 * Return the value of the semaphore <semid>
 */
int sem_getval ( int semid )
{
    union semun info;

    return semctl(semid, 0, GETVAL, info);
}

/*
 * Apply the wait operation to the semaphore <semid>.
 * Note the flag value - we don't use SEM_UNDO here since
 * we will *not* be using the signal operation on the semaphore,
 * it is being done by someone else so we don't want our wait
 * to be undone when we terminate.
 */
int sem_wait ( int semid ) 
{
    struct sembuf wait[1] = { {0, -1, 0} };
  
    return ( semop(semid, wait, 1) );
}

/*
 * Apply the signal operation to the semaphore <semid>.
 * Similar comments to sem_wait, re. SEM_UNDO
 */
int sem_signal ( int semid )
{
    struct sembuf signal[1] = { {0, 1, 0} };
    
    return ( semop(semid, signal, 1) );
}

/*
 * Remove the semaphore <semid>
 */
int sem_remove ( int semid )
{
    return semctl(semid, 0, IPC_RMID, 0);
}

