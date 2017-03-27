#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include "myrec.h"

#define FILENAME "database"

/* 
 * Program to create the database file for exercising random I/O.
 * Writes a series of 64 records of the same size to the file. Each
 * record contains a collection of miscellaneous data, including some
 * binary information that should make it hard to "guess" what's in a
 * record without having read it first...
 */

int main (int  argc, char *argv[])
{
  int  i; 
  size_t bytes;
  int fd;
  char ch='A';
  struct myrec theRecord;

  /* 
   * Open the file for writing...
   */
  fd = open (FILENAME, O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
  if (fd < 0) {
    perror("can't create file\n");
    exit(1);
  }

  /*
   * Seed the random number generator used in filling the record data...
   */
  srand(1234L);

  for (i=0; i<64; i++) {
    /* 
     * Initialise an empty record
     */
    memset(&theRecord, '\0', sizeof(theRecord));

    /*
     * Fill in the data (mostly meaningless...)
     */
    theRecord.index = i+1;
    theRecord.dataval = rand() % 1024;
    strcpy ( theRecord.label, "Label: " );
    theRecord.label[strlen(theRecord.label)] = ch++;

    /* 
     * Write the record to the file 
     */
    bytes = write (fd, &theRecord, sizeof(theRecord)); 
    if (bytes != sizeof(theRecord)) 
      {
	perror("write error");
	exit(1);
      }
  }

  close(fd);
  exit(0);
}
