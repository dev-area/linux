#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "myrec.h"

#define FILENAME "database"

/*
 * Retrieve the record at "index" specified on the command line,
 * update is data value (add 1) and then write it back...
 */
int main (int  argc, char *argv[])
{
  int recno, i, n;
  size_t bytes;
  int fd;
  struct myrec theRecord;

  /* 
   * Usage check - exactly one parameter
   */
  if (argc != 2) {
    fprintf (stderr, "invalid number of parameters\n");
    fprintf (stderr, "Syntax: %s record\n", argv[0]);
    exit(1);
  }

  /* 
   * Retrieve record number - the first argument...
   */

  n = atoi(argv[1]);
  if (n <= 0) {
    fprintf (stderr, "Cannot access record number %s\n", argv[1]);
    fprintf (stderr, "Please supply a positive number\n");
    exit(1);
  }

  /*
   * The file must be open for read and write
   */
  if ( (fd = open (FILENAME, O_RDWR))< 0) {
    perror("can't open file");
    exit(1);
  }

  /* 
   * Position the read pointer at the selected record
   */
  if (lseek(fd,(n-1)*(sizeof(struct myrec)),SEEK_SET) < 0) {
    perror("can't seek to record");
    exit(1);
  }

  /* read selected record */
  bytes = read (fd, &theRecord, sizeof(theRecord)); 
  if (bytes != sizeof(theRecord)) {
    perror("read error");
    exit(1);
  }

  /* 
   * Display the record contents
   */
  printf("Index: %d\nData: %d\nLabel: %s\n",
	 theRecord.index, theRecord.dataval, theRecord.label);

  /* 
   * Now update the value and write it back...
   */
  theRecord.dataval++;

  /* 
   * Write record to stdout again to show the new values that
   * should be written back
   */
  printf("After change, Index: %d\nData: %d\nLabel: %s\n",
	 theRecord.index, theRecord.dataval, theRecord.label);
 
  /*
   * And here's the trap! We must leek() back to the original
   * record position or else we will overwrite the next rec in the file...
   */
  if (lseek(fd,(n-1)*(sizeof(struct myrec)),SEEK_SET) < 0) {
    perror("can't seek to record");
    exit(1);
  }

  if ( write(fd, &theRecord, sizeof(theRecord)) != sizeof(theRecord) ) {
    perror("write failure");
    exit(1);
  }
  close(fd);
  exit(0);
}
