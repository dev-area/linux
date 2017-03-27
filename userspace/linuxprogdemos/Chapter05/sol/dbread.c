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
 * Read and display a record from the database file
 * create by the dbwrite program. The record number should
 * be supplied on the command line
 */
int main (int  argc, char *argv[])
{
  int recno, i, length;
  struct stat sbuf;
  size_t bytes;
  int fd;
  struct myrec *records;
                                    
  /*
   * Usage check: should be exactly one parameter
   */
  if (argc != 2) {
    fprintf (stderr, "Syntax: %s record\n", argv[0]);
    exit(1);
  }

  /*
   * Record number is the first argument, but it needs to be
   * converted from string to integer
   */
  recno = atoi(argv[1]);
  if (recno <= 0) {
    fprintf (stderr, "Cannot access record number %s\n", argv[1]);
    exit(1);
  }

  if ( recno > MAXRECNO ) {
    fprintf(stderr, "Record number must be less than %d\n", MAXRECNO);
    exit(1);
  }

  /*
   * The array of records is 0 based, so take 1 from requested record
   * number to get the right one
   */
  recno--;

  /*
   * Open the file for reading, and find out its length
   */
  if ((fd = open (FILENAME, O_RDONLY)) < 0) {
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
  records = (struct myrec *)mmap(0, length, PROT_READ, 
				 MAP_PRIVATE, fd, 0);
  if ( records == (struct myrec *)-1 ) {
    perror("mmap");
    exit(1);
  }
  close (fd);

  /*
   * Now write the information held in the record, which will be
   * at the correct index in the array...
   */
  printf("Index: %d\nData: %d\nLabel: %s\n",
         records[recno].index, records[recno].dataval, records[recno].label);

  exit(0);

}


  


