#include <curses.h>
#include <sys/time.h>
#include <signal.h>

main ( int argc, char *argv[] )
{
  WINDOW *theWindow = initscr();

  /*
   * Your stuff has to go here...
   */
}


/*
 * Use this fucntion to draw an "image", as represented by
 * the string "im".
 */  
int drawimage ( char *im )
{
  mvaddstr(0, 0, im);
  refresh();
}

/*
 * Use this function to clean up when the program has to terminate
 */
void done ( int sig ) 
{
  char *msg = "Bye for now...\n";
  endwin();
  write(1,msg, strlen(msg));
  exit(0);
}
