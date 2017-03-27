
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main ()
{
      int md;
      int status;
      long pg_size;
      int * virt_addr;

      /* Create shared memory object */

      md = shm_open("/my_memory", O_CREAT|O_RDWR, 0);
      ftruncate(md, 1000);
      virt_addr = mmap(0, pg_size, PROT_WRITE|PROT_READ, MAP_SHARED, md, 0);
      
      virt_addr[10]=30;
      
      
      
      status = munmap(virt_addr, pg_size);  /* Unmap the page */
      status = close(md);                   /*   Close file   */
      status = shm_unlink("my_memory");     /* Unlink shared-memory object */
	  return 0;
}

