#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
 * A simple interactive command interpreter.
 * No built-ins (except "exit") and no fancy I/O manipulation.
 */

#define MAXLINE 80
#define MAXARGS 16

main ( int argc, char *argv[])
{
  char *prompt = "=> ";
  char linebuf[MAXLINE], *line = linebuf;
  char *argp[MAXARGS];
  int cpid, status;

  /*
   * Display prompt and read command from user.
   * If fgets() returns NULL it means user typed Ctrl-D so we
   * terminate, just like standard shells.
   */
  fputs(prompt, stdout); fflush(stdout);

  while ( (line = fgets(linebuf, MAXLINE, stdin)) != NULL ) {

      /*
       * Convert line into array of strings, the args
       */
    parseCommand(line, argp);


    if ( argp[0] ) {

	/*
	 * Check for exit command
	 */
      if ( strcmp(argp[0], "exit") == 0 )
	exit(0);
      
      /*
       * Create child process to execute command
       */
      switch ( cpid = fork() ) {
      case -1:
	perror("fork");
	exit(1);
      case 0:
	/* 
	 * Child - simple execvp call is all we need.
	 */
	execvp(argp[0], argp);
	perror("exec");
	exit(1);
      default:
	/*
	 * Parent - wait for child. Don't report anything if
	 * child exited.
	 */
	if ( waitpid(cpid, &status, 0) != cpid ) {
	  perror("waitpid");
	  exit(1);
	}

	/*
	 * If child was signalled, say which signal.
	 */
	if ( WIFSIGNALED(status) ) 
	  printf("Child cmd in process %d was signalled with signal %d\n", 
		 cpid, WTERMSIG(status));
      }
    }

    /*
     * Prompt and back round again...
     */
    fputs(prompt, stdout); fflush(stdout);
  }
}

/*
 * Take string containing command and break it into
 * "words", using white space as word break chars.
 * Each word is a separate string, referenced through the
 * array args, which is itself NULL terminated.
 * This makes it just like argv, which is what we want, after all.
 */
int parseCommand(char *buf, char *args[])
{
  char *separators = " \t\n";
  char *cp = strtok(buf, separators);
  int argIndex = 0;

  while ( cp ) {
    args[argIndex++] = cp;
    cp = strtok(NULL, separators);
  }

  args[argIndex] = NULL;
  
  return 0;
}
