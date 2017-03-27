#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
 * Create a child and then report on its demise...
 */
main ( int argc, char *argv[] )
{
  int cpid;
  int ecode;
  int status;

  /*
   * Argument is the exit code to be used by the child.
   */
  if ( argc > 1 )
    ecode = atoi(argv[1]);
  else 
    ecode = 0;

  printf ( "Parent process has pid %d\n", getpid());

  switch ( cpid = fork() ) {

  case -1:
    perror("fork");
    exit(1);

  case 0:
    /*
     * Child - sleep 5 seconds and then exit with supplied status
     */
    printf("Child process - pid %d\n", getpid());
    sleep(5);
    exit(ecode);

  default:
    /*
     * Parent - wait for child and report its status
     */
    if ( waitpid(cpid, &status, 0) != cpid ) {
      perror("waitpid");
      exit(1);
    }

    if ( WIFEXITED(status) )
	/*
	 * Normal exit, report exit status 
	 */
      printf("Child %d exited with status %d\n", cpid,
	     WEXITSTATUS(status));
    else if ( WIFSIGNALED(status) )
	/*
	 * Signal-induced exit - report signal number
	 */
      printf("Child %d was signalled with signal %d\n", cpid,
	     WTERMSIG(status));
    else
	/*
	 * Dont want to know here...
	 */
      printf("Something unpleasant happened to child %d\n", cpid);
  }

  exit(0);
}
