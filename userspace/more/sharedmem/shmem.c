
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
struct mydata{
	int arr[1000];
	int num;
	sem_t s1;
};

int main ()
{
      int md;
      int status;
      long pg_size;
      struct mydata * virt_addr;

      /* Create shared memory object */

      md = shm_open("/my_memory", O_TRUNC|O_CREAT|O_RDWR, 0660);
      ftruncate(md, sizeof(struct mydata));
      virt_addr = mmap(0, sizeof(struct mydata), PROT_WRITE|PROT_READ, MAP_SHARED, md, 0);
     
      sem_init(&virt_addr->s1, 1, 0);
      sleep(10);
      virt_addr->num=0;
      while(1)
      {
      	sleep(2);
      	virt_addr->num +=10;
      	sem_post(&virt_addr->s1);
      }
      
      
      
      status = munmap(virt_addr, pg_size);  /* Unmap the page */
      status = close(md);                   /*   Close file   */
      status = shm_unlink("my_memory");     /* Unlink shared-memory object */
	  return 0;
}

