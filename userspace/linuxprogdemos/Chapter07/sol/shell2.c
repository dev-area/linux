#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

/*
 * A simple interactive command interpreter, modified to
 * allow I/O redirection.
 */

#define MAXLINE 80
#define MAXARGS 16

main ( int argc, char *argv[])
{
    char *prompt = "=> ";
    char linebuf[MAXLINE], *line = linebuf;
    char *argp[MAXARGS];
    int cpid, status;
    int ofd, ifd;

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
		 * Child - see if I/O redirection is required before we
		 * execute the command.
		 */
		switch ( ofd = doOutputRedirect(argp) ) {
		case -1:
		    exit(1);
		case 0:
		    break;
		default:
		    if ( dup2(ofd, 1) < 0 ) {
			perror("Unable to redirect");
			exit(1);
		    }
		}

		switch ( ifd = doInputRedirect(argp) ) {
		case -1:
		    exit(1);
		case 0:
		    break;
		default:
		    if ( dup2(ifd, 0) < 0 ) {
			perror("Unable to redirect");
			exit(1);
		    }
		}
	  
		execvp(argp[0], argp);
		perror("exec");
		exit(1);
	    default:
		/*
		 * Parent - wait for child.
		 */
		if ( waitpid(cpid, &status, 0) != cpid ) {
		    perror("waitpid");
		    exit(1);
		}

		/*
		 * Report if child was terminated by a signal
		 */
		if ( WIFSIGNALED(status) ) 
		    printf("Child cmd in process %d was signalled with signal %d\n", 
			   cpid, WTERMSIG(status));
	    }
	}

	/*
	 * Round again...
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


/* 
 * Check for output redirection.
 * We either find an arg like ">foo", or one like ">"
 * followed by one that is a filename.
 * Return descriptor representing the correct file opened (created
 * if necessary) for writing, or 0 if no redirection, or -1 if
 * there was a problem.
 */
int doOutputRedirect ( char *args[] )
{
    int argindx=0, fd, offs;

    while ( args[argindx] && *(args[argindx]) != '>' ) {
	argindx++;
    }

    if (  args[argindx] ) {
	/*
	 * We found an arg beginning with '>'.
	 * Open the file for output
	 */
	char *oFile;

	if ( *(args[argindx]+1) != '\0' ) {

	    /*
	     * The arg was like ">foo"
	     */
	    offs = 1;
	    oFile = args[argindx]+1;
	    
	} else {

	    /*
	     * The filename is in the next arg (we hope!)
	     */
	    offs = 2;
	    if ( args[argindx+1] )
		oFile = args[argindx+1];
	    else {
		fprintf(stderr, "Redirection syntax error\n");
		return(-1);
	    }
	}

	/*
	 * Open the file for writing, create if necessary.
	 */
	if ( (fd = open(oFile, O_WRONLY|O_CREAT, 0666)) < 0 ) {
	    perror(oFile);

	} else {

	    /*
	     * File is ready, remove the redirection args from
	     * the arg array as the child command should never
	     * see them
	     */
	    while ( args[argindx+offs] ) {
		args[argindx] = args[argindx+offs];
		argindx++;
	    }
	    args[argindx] = args[argindx+offs];
	}

	/*
	 * Return descriptor, or -1 if we couldn't open it...
	 */
	return ( fd );

    } else

	/*
	 * Here means there was no redirection
	 */
	return (0);
}



/* 
 * Check for input redirection.
 * We either find an arg like "<foo", or one like "<"
 * followed by one that is a filename.
 * Return descriptor representing the correct file opened for reading, 
 * or 0 if no redirection, or -1 if there was a problem.
 */
int doInputRedirect ( char *args[] )
{
    int argindx=0, fd, offs;

    while ( args[argindx] && *(args[argindx]) != '<' ) {
	argindx++;
    }

    if (  args[argindx] ) {
	/*
	 * We found an arg beginning with '<'.
	 * Open the file following for output
	 */
	char *iFile;

	if ( *(args[argindx]+1) != '\0' ) {

	    /*
	     * The arg was like ">foo"
	     */
	    offs = 1;
	    iFile = args[argindx]+1;

	} else {
	    /*
	     * The filename is in the next arg (we hope!)
	     */
	    offs = 2;
	    if ( args[argindx+1] )
		iFile = args[argindx+1];
	    else {
		fprintf(stderr, "Redirection syntax error\n");
		return(-1);
	    }
	}
 
	/*
	 * Open the file for readingy.
	 */
	if ( (fd = open(iFile, O_RDONLY)) < 0 ) {
	    perror(iFile);
	} else {

	    /*
	     * File is ready, remove the redirection args from
	     * the arg array as the child command should never
	     * see them
	     */
	    while ( args[argindx+offs] ) {
		args[argindx] = args[argindx+offs];
		argindx++;
	    }
	    args[argindx] = args[argindx+offs];
	}

	/*
	 * Return descriptor, or -1 if we couldn't open it...
	 */
	return ( fd );
    } else

	/*
	 * Here means there was no redirection
	 */
	return (0);
}
