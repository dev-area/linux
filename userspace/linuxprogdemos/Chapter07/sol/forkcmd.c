#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
 * Create a child process that executes the command that
 * has been passed as arguments.
 */
main ( int argc, char *argv[] )
{
  int cpid;
  int status;
  char **ap = argv+1;

  if ( argc == 1 ) {
    fprintf(stderr, "Usage: %s cmd arg ...\n", argv[0]);
    exit(1);
  }

  printf ( "Parent process has pid %d\n", getpid());

  switch ( cpid = fork() ) {

  case -1:
    perror("fork");
    exit(1);

  case 0:
    /*
     * Child - have it exec() the command that is specified by 
     * our arguments. 
     * execvp() is the most convenient of the functions to use as
     * the args are already in "vector" form, and we'd like to look
     * up the command on the path.
     */
    printf("Child process - pid %d\n", getpid());
    execvp(ap[0], ap);
    perror("exec");
    exit(1);

  default:
    /*
     * Parent - wait for child to terminate and collect its status
     */
    if ( waitpid(cpid, &status, 0) != cpid ) {
      perror("waitpid");
      exit(1);
    }

    if ( WIFEXITED(status) )
      printf("Child %d exited with status %d\n", cpid,
	     WEXITSTATUS(status));
    else if ( WIFSIGNALED(status) ) 
      printf("Child %d was signalled with signal %d\n", cpid,
	     WTERMSIG(status));
    else
      printf("Something unpleasant happened to child %d\n", cpid);
  }

  exit(0);
}
