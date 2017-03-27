#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAXLINELEN 128
#define OUTPUTFILE "./cmd.out"

/*
 * Execute command specified on the command line in a child
 * process, place its standard output in a file, counting the 
 * number of lines on the way...
 */
main ( int argc, char *argv[] )
{
  int cpid;
  int status;
  int pipeFds[2];
  char **ap = argv+1;
  FILE *pipeHandle, *outFile;
  int lines;
  char lineBuf[MAXLINELEN];

  if ( argc == 1 ) {
    fprintf(stderr, "Usage: %s cmd arg ...\n", argv[0]);
    exit(1);
  }

  /*
   * Create the pipe
   */
  if ( pipe(pipeFds) < 0 ) {
    perror("pipe");
    exit(1);
  }

  switch ( cpid = fork() ) {

  case -1:
    perror("fork");
    exit(1);

  case 0:
    /*
     * Child - set up pipe so we write our standard output to it.
     * Close read end of the pipe.
     */
    close(pipeFds[0]);
    if ( dup2(pipeFds[1], 1) < 0 ) {
      perror("dup2");
      exit(1);
    }
    execvp(ap[0], ap);
    perror("exec");
    exit(1);

  default:
    /*
     * Parent - close write end of the pipe.
     */
    close(pipeFds[1]);

    /*
     * Set up read end of the pipe so that it can be read using
     * stdio functions.
     */
    if ( (pipeHandle = fdopen(pipeFds[0], "r")) == (FILE *)NULL ) {
	fprintf(stderr, "Unable to create file handle from pipe\n");
	exit(1);
    }
    
    /*
     * Open the file to contain the actual output from the child.
     */
    if ( (outFile = fopen(OUTPUTFILE, "w")) == (FILE *)NULL ) {
	fprintf(stderr, "Unable to create file handle for output\n");
	exit(1);
    }
    
    /*
     * Read from the pipe, a line at a time, count lines and
     * then copy them to the output file.
     */
    lines=0;
    while ( fgets(lineBuf, MAXLINELEN, pipeHandle) != NULL ) {
	lines++;
	fputs(lineBuf, outFile);
    }
    fclose(outFile);


    /*
     * Wait for the child and clean up after it...
     */
    if ( waitpid(cpid, &status, 0) != cpid ) {
      perror("waitpid");
      exit(1);
    }

    if ( WIFEXITED(status) ) {
      printf("Child %d exited with status %d\n", cpid,
	     WEXITSTATUS(status));
      printf("%d lines of output were added to file %s\n", lines, OUTPUTFILE);
    } else if ( WIFSIGNALED(status) ) {
      printf("Child %d was signalled with signal %d\n", cpid,
	     WTERMSIG(status));
      printf("%d lines of output were added to file %s\n", lines, OUTPUTFILE);
    } else
      printf("Something unpleasant happened to child %d\n", cpid);

  }

  exit(0);
}
