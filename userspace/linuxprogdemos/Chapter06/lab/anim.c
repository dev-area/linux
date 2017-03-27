#include <curses.h>
#include <sys/time.h>
#include <signal.h>
#include "images.h"

main ( int argc, char *argv[] )
{
  WINDOW *theWindow = initscr();

  /*
   * Your stuff has to go here...
   */
}
  
/*
 * Use this function to draw an "image", represented as a string
 */
int drawimage ( char *im )
{
  mvaddstr(0, 0, im);
  refresh();
}


/*
 * Use this function to tidy up when all is done...
 */
void done ( int sig ) 
{
  char *msg = "Bye for now...\n";
  endwin();
  write(1,msg, strlen(msg));
  exit(0);
}
