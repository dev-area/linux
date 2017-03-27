/*
   popen.c  Chapter 8

*/

#include <stdio.h>

#define MAXBUF 2048

main ( int argc, char *argv[] )
{
  char buffer[MAXBUF];
  FILE *fp = popen("gzip -dc data.gz","r");

  while (fgets(buffer,MAXBUF,fp)) 
  {
    /* Process line of data, here just print it out */
    fputs(buffer,stdout);
  }

  printf ("Command exit status %d\n", pclose(fp));

}
