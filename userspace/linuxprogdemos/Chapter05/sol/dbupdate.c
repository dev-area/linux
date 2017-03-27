#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "myrec.h"

/*
 * Use hard-wired filename for now.
 * There will be 64 records in the file, so the max number we
 * can have is 63...
 */
#define FILENAME "database"
#define MAXRECNO 64

/*
 * Update the data field of record(s) read from the file.
 * In order to allow measurement of relative performances between this and
 * the original lseek() based version, we process all records in the file
 * a given number of times...
 * Otherwise it will all be over too quickly ;-)
 */
int main (int  argc, char *argv[])
{
  int recno, i, num_iter, length;
  struct stat sbuf;
  size_t bytes;
  int fd;
  struct myrec *records;
                                    
  /*
   * Usage check: should be exactly one parameter
   */
  if (argc != 2) {
    fprintf (stderr, "Syntax: %s num_iter\n", argv[0]);
    exit(1);
  }

  /*
   * Argument is the number of times that we will scan through the
   * records in the file.
   */
  num_iter = atoi(argv[1]);
  if (num_iter <= 0) {
    fprintf (stderr, "Please give positive number of iterations!\n");
    exit(1);
  }

  /*
   * Open the file for reading/writing, and find out its length
   */
  if ((fd = open (FILENAME, O_RDWR)) < 0) {
    perror(FILENAME);
    exit(1);
  }
  if ( fstat(fd, &sbuf) < 0 ) {
    perror("stat");
    exit(1);
  }
  length = sbuf.st_size;

  /*
   * Now map it into our address space so it appears as an
   * array of struct myrec
   */
  records = (struct myrec *)mmap(0, length, PROT_READ|PROT_WRITE, 
				 MAP_SHARED, fd, 0);
  if ( records == (struct myrec *)-1 ) {
    perror("mmap");
    exit(1);
  }
  close (fd);

  /*
   * Now go through all records in the file updating their data field...
   */
  while ( num_iter-- ) {
    int recno;
    for ( recno=0; recno<MAXRECNO; recno++ ) {
      records[recno].dataval++;

      printf("Index: %d\tData: %d\tLabel: %s\n",
	     records[recno].index, records[recno].dataval,
	     records[recno].label);
    }
  }

  exit(0);

}


  


