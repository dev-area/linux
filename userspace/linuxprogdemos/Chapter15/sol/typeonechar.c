#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

/*
 * Simple test program for typeonechar function that holds up activity until
 * a single character is typed. The character may be anything, and
 * will not be echoed.
 */

void typeonechar();

int main (void)
{
    printf ("This is just a simple wee test harness...\n");
    typeonechar();
    printf("Thank you!");
    exit(0);
}

/*
 * Function to hold up activity until a single character is typed. 
 * The character may be anything, and will not be echoed.
 */
void typeonechar ()
{
    char *prompt = "Press any character to continue...";
    struct termios old, new;
    char ch;

    /*
     * Print prompt. To stdout for now...
     */
    write(STDOUT_FILENO, prompt, strlen(prompt));

    /*
     * Retrieve current terminal settings (so we can restore them afterwards)
     */
    if (tcgetattr(STDIN_FILENO, &old) < 0) {
        perror ("tcgetattr failed");
        exit (1);
    }

    /*
     * Set up new settings:
     * Turn off canonical mode and all echoing.
     * Set character read controls to wait until the first character
     * is typed, then return it.
     */
    new = old;
    new.c_lflag &= ~(ICANON|ECHO|ECHOE|ECHOK);
    new.c_cc[VMIN] = 1;
    new.c_cc[VTIME] = 0;

    /*
     * Install new settings
     */
    if (tcsetattr(STDIN_FILENO,TCSAFLUSH,&new) < 0) {
        perror ("tcsetattr failed");
        exit (1);
    }

    /*
     * Now read the character. We will wait until one is pressed.
     */
    while ( read(STDIN_FILENO, &ch, 1) < 0 ) {
	if ( errno != EINTR ) {
	    tcsetattr (STDIN_FILENO, TCSAFLUSH, &old);
	    exit(1);
	}
    }
 
    /*
     * Restore terminal settings on stdin before we return.
     */
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &old);
    return;
}
