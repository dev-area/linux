#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "myrec.h"

#define FILENAME "database"

/* 
 * Read and display a record from the database file
 * create by the dbwrite program. The record number should
 * be supplied on the command line
 */
int main (int  argc, char *argv[])
{
  int recno, i, n;
  size_t bytes;
  int fd;
  struct myrec theRecord;

  /* 
   * Usage check: should be exactly one parameter
   */
  if (argc != 2) {
    fprintf (stderr, "Usage: %s record\n", argv[0]);
    exit(1);
  }

  /* 
   * Record number is the first argument, but it needs to be
   * converted from string to integer
   */
  n = atoi(argv[1]);
  if (n <= 0) {
    fprintf (stderr, "Cannot access record number %s\n", argv[1]);
    fprintf (stderr, "Please supply a positive number\n");
    exit(1);
  }

  /* 
   * Open the file for reading
   */
  if ((fd = open (FILENAME, O_RDONLY)) < 0) {
    perror("can't open file");
    exit(1);
  }

  /* 
   * Position the read pointer at the selected record offset
   */
  if (lseek(fd,(n-1)*(sizeof(struct myrec)),SEEK_SET) < 0) {
    perror("can't seek to record");
    exit(1);
  }

  /* 
   * Now read the record 
   */
  bytes = read (fd, &theRecord, sizeof(theRecord)); 
  if (bytes != sizeof(theRecord)) {
    perror("read error");
    exit(1);
  }

  /* 
   * Write the information held in the record
   */
  printf("Index: %d\nData: %d\nLabel: %s\n",
	 theRecord.index, theRecord.dataval, theRecord.label);

  close(fd);
  exit(0);
}
