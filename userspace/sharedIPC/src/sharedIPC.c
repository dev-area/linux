#include <semaphore.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/mman.h> 
#include <stdio.h> 

int main () 
{ 
   char *path = "tshmem"; 
   int i, pid, fd, size = sizeof(sem_t); 
   void *ptr; 
   sem_t *sem; 
    
   if ((pid = fork()) == -1) 
      return -1; 
    
   if (pid) 
   { 
      fd = shm_open(path, O_RDWR|O_CREAT, 0777); 
      ftruncate(fd, size); 
      ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0); 
      sem = ptr; 
      sem_init(sem, 1, 0); 

      sleep(1); 
      sem_post(sem); 

      for (i = 0; i < 5; i++) 
      { 
         sem_wait(sem); 
         sleep(1);
         printf("%d parent process - inside critical section\n", getpid()); 
         sem_post(sem); 
      } 
   } 
   else 
   { 
      sleep(1); 
       
      fd = shm_open(path, O_RDWR, 0777); 
      ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0); 
      sem = ptr; 

      for (i = 0; i < 5; i++) 
      { 
         sem_wait(sem); 
         printf("%d child process - inside critical section\n", getpid()); 
         sleep(1); 
         sem_post(sem); 
      } 
   } 
    
   munmap(ptr, size); 
   close(fd); 
   shm_unlink(path); 
    
   return 0; 
} 
 
