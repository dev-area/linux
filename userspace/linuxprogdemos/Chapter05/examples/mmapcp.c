/*
   mmapcp.c  Chapter 5

*/

#define NOMAP ((caddr_t) -1)
#define PERMS (S_IRWXU|S_IRWXG|S_IRWXO)

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
   int target, source, len;
   char *src, *dest;
   struct stat statbuf;

   if (argc != 3)
   {
     fprintf(stderr,"Usage: %s source dest\n",argv[0]);
     exit(1);
   }

   source = open (argv[1], O_RDONLY);
   /* Should test return from open() here! */

   fstat (source, &statbuf);
   len = statbuf.st_size;
   target = open(argv[2],O_RDWR|O_CREAT,statbuf.st_mode&PERMS);
   /* Should test return from open() here! */

   ftruncate (target, len);

   src = mmap (0,len,PROT_READ,MAP_PRIVATE,source,0);
   if (src == NOMAP) 
   { 
      perror("can't map source file"); 
      exit(1); 
   }
 
   dest = mmap (0,len,PROT_READ|PROT_WRITE,MAP_SHARED,target,0);
   
   if (dest == NOMAP)
   { 
      perror("can't map target file"); 
      exit(1); 
   }
   
   close (source); 
   close (target);
   
   memcpy (dest, src, len);

}
