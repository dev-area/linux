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

  struct sigaction action;
  sigset_t mask;
  struct itimerval interval;
  int factor;

  void tick ();
  void done ();

  /*
   * Allow the user to specify an animation speed "factor",
   * what it actually means is that the signal is delivered every
   * 1/factor of a second.
   * So higher number is faster animation.
   * Default to 1/10 second.
   */
  if ( argc < 2 ) {
    factor = 10;
  } else {
    factor = atoi(argv[1]);
  }
  if ( factor == 0 ) 
    factor = 10;

  /*
   * Set up and install alarm signal handler
   */
  action.sa_handler = tick;
  sigemptyset(&mask);
  action.sa_mask = mask;
  action.sa_flags = 0;
  if ( sigaction(SIGALRM, &action, NULL) < 0 ) {
    perror("sigaction(ALRM)");
    exit(1);
  }

  /*
   * Set up and install SIGINT handler so we tidy up correctly.
   */
  action.sa_handler = done;
  if ( sigaction(SIGINT, &action, NULL) < 0 ) {
    perror("sigaction(INT)");
    exit(1);
  }

  /*
   * Set up interval timer, an initial wait of 1 second, then
   * the rate of interrupts depends on the value of "factor"
   */
  interval.it_interval.tv_sec = 0;
  interval.it_interval.tv_usec = 1000000/factor;
  interval.it_value.tv_sec = 1;
  interval.it_value.tv_usec = 0;
  if ( setitimer(ITIMER_REAL, &interval, NULL) < 0 ) {
    perror("setitimer");
	endwin();
    exit(1);
  }

  /*
   * Now we just sit and wait...
   */
  for ( ;; ) ;

  endwin();
}
  

/*
 * Handler for SIGALRM, draw the next image in sequence, 
 * wrapping round when we get to the end of the array.
 */
void tick ( int sig ) 
{
  static int img = 0;

  drawimage(images[(img++)%4]);
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
