/*
   linkinfo.c  Chapter 3
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main (int argc, char *argv[])
{
   struct stat statbuf;
   char buffer[FILENAME_MAX+1];
   int i, n;

   for (i=1; i<argc; i++) 
   {  
      if (lstat(argv[i],&statbuf) == -1)
      { 
         perror(argv[i]); 
         continue; 
      }

      printf ("%s", argv[i]);

      if (S_ISLNK(statbuf.st_mode))
      {
         if ((n=readlink(argv[i],buffer,FILENAME_MAX)) == -1 )
         {
          perror(argv[i]); 
          exit(1); 
         } 
         buffer[n] = '\0';
         printf ("->%s", buffer);
      }

      putchar ('\n');

   }
}
