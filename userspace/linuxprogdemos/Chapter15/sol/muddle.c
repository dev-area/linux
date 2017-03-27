#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

/*
 * Read characters from user on stdin, but echo back something
 * different from what they typed...
 */
#define MAXBUF 80

int main (void)
{
    int  i;
    char ch;
    struct termios old;
    struct termios new;
    char buffer[MAXBUF + 1];

    /*
     * Get current settings.
     */
    if (tcgetattr(STDIN_FILENO,&old) < 0) {
        perror ("tcgetattr failed");
        exit (1);
    }

    /*
     * Change settings so there is no echoing or canonical processing. 
     * This is because we need access to every character as
     * it is typed, and we will echo back what we want to...
     */
    new = old;
    new.c_lflag &= ~(ICANON|ECHO|ECHOE|ECHOK);
    new.c_cc[VMIN] = 1;
    new.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO,TCSAFLUSH,&new) < 0) {
        perror ("tcsetattr failed");
        exit (1);
    }

    /*
     * Prompt
     */
    i = 0;
    printf ("Enter data? ");
    fflush (stdout);

    for (;;)  {

	/*
	 * Read a character, just <CR> to finish
	 */
        if (read(STDIN_FILENO,&buffer[i],1) <= 0)
            break;

	/*
	 * Store it
	 */
        ch = buffer[i++];

	/*
	 * If new line, then display what they actually typed
	 * and start again.
	 */
    	if (ch == '\n' || i >= MAXBUF) {
    	    putchar('\n');
    	    
    	    if (i == 1)
    	        break;

	    buffer[i-1] = '\0';
    	    printf ("You actually typed \"%s\"\n", buffer);

    	    i = 0;
	    printf ("Next line? ");
	    fflush (stdout);

    	} else {

	    /*
	     * Generate a random character based on the one that
	     * was typed.
	     */
    	    srand(ch);
    	    ch = rand()%96 + ' ';
    	    if (write(STDOUT_FILENO,&ch,1) != 1) {
    	        perror ("write error");
    	        break;
    	    }
    	}

    }

    /*
     * Restore terminal settings
     */
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &old);

    exit(0);
}
