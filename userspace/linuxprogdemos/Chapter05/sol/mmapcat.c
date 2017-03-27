#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>

/*
 * A version of the "cat" program that uses mmap
 * to gain access to the file being listed.
 */
int main ( int argc, char *argv[] )
{
  int fd, length;
  struct stat sbuf;
  char *data;

  /*
   * Usage check - allow just one file at a time
   */
  if ( argc != 2 ) {
    fprintf(stderr, "Usage: %s file\n", argv[0]);
    exit(1);
  }

  /*
   * The file needs to be opened, then we need its size.
   * Should also check that it's a regular file - mmap won't
   * work on other stuff...
   */
  if ( (fd = open(argv[1], O_RDONLY)) < 0 ) {
    perror(argv[1]);
    exit(1);
  }
  if ( fstat(fd, &sbuf) < 0 ) {
    perror("stat");
    exit(1);
  }
  if ( sbuf.st_mode & S_IFMT != S_IFREG ) {
      fprintf(stderr, "%s: not a reagular file\n", argv[1]);
      exit(1);
  }
  length = sbuf.st_size;

  /* 
   * Map the file into our address space, we should get a pointer to
   * the start of the file data.
   */
  data = mmap(0, length, PROT_READ, MAP_PRIVATE, fd, 0);
  if ( data == (char *)-1 ) {
    perror("mmap");
    exit(1);
  }

  /*
   * Don't need the file open any more
   */
  close ( fd );

  /*
   * Now we simply copy the contents of the file to stdout. It appears
   * like an area of our address space, so one call to write() does the
   * job for us...
   */
  write(fileno(stdout), data, length);

  exit(0);
}
