
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

      md = shm_open("/my_memory", O_RDWR,0);
      virt_addr = mmap(0, sizeof(struct mydata), PROT_WRITE|PROT_READ, MAP_SHARED, md, 0);

      while(1)
      {
      	//sleep(2);
      	sem_wait(&virt_addr->s1);
      	printf("val=%d\n",virt_addr->num);
      }
      
      
      
      status = munmap(virt_addr, pg_size);  /* Unmap the page */
      status = close(md);                   /*   Close file   */
      status = shm_unlink("my_memory");     /* Unlink shared-memory object */
	  return 0;
}

