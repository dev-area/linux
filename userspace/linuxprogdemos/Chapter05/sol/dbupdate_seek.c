#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "myrec.h"

#define FILENAME "database"
#define MAXRECNO 63

/*
 * Update data fields of records
 */
int main (int  argc, char *argv[])
{
  int recno, i, num_iter;
  size_t bytes;
  int fd;
  struct myrec theRecord;

  /* 
   * Usage check - exactly one parameter
   */
  if (argc != 2) {
    fprintf (stderr, "Syntax: %s num_iterations\n", argv[0]);
    exit(1);
  }

  /* 
   * Retrieve the required number of iterations
   */

  num_iter = atoi(argv[1]);
  if (num_iter <= 0) {
    fprintf (stderr, "Please gove a positive number of iterations!\n");
    exit(1);
  }

  /*
   * The file must be open for read and write
   */
  if ( (fd = open (FILENAME, O_RDWR))< 0) {
    perror("can't open file");
    exit(1);
  }

  while ( num_iter-- ) {
    int recno;

    for ( recno=0; recno <= MAXRECNO; recno++ ) {
      /* 
       * Position the read pointer at the selected record
       */
      if (lseek(fd,(recno)*(sizeof(struct myrec)),SEEK_SET) < 0) {
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
     * Update the value and write it back...
     */
      theRecord.dataval++;

      /* 
     * Display the record contents after the change...
     */
      printf("Index: %d\tData: %d\tLabel: %s\n",
	     theRecord.index, theRecord.dataval, theRecord.label);
 
      /*
     * Remember to leek() back to the original
     * record position or else we will overwrite the next rec in the file...
     */
      if (lseek(fd,(recno)*(sizeof(struct myrec)),SEEK_SET) < 0) {
	perror("can't seek to record");
	exit(1);
      }

      if ( write(fd, &theRecord, sizeof(theRecord)) != sizeof(theRecord) ) {
	perror("write failure");
	exit(1);
      }
    }
  }
  close(fd);
  exit(0);
}
