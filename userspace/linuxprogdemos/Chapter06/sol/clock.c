#include <curses.h>
#include <sys/time.h>
#include <signal.h>

main ( int argc, char *argv[] )
{
  WINDOW *theWindow = initscr();
  /*
   * Your stuff has to go here...
   */

  struct sigaction action;
  sigset_t mask;
  struct itimerval interval;

  void tick ();
  void done ();

  /*
   * Set up and install handler for SIGALRM, the timer's signal
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
   * Set up and install handler for SIGINT, this will be used to 
   * terminate the program. The handler tidies up and resets the
   * terminal settings so we can carry on using the terminal.
   */
  action.sa_handler = done;
  if ( sigaction(SIGINT, &action, NULL) < 0 ) {
    perror("sigaction(INT)");
    exit(1);
  }

  /*
   * Interval timer setup, we want to be signalled every second, 
   * so both intervals set to 1 second.
   * Use wall clock time so the timer to set is ITIMER_REAL.
   */
  interval.it_interval.tv_sec = 1;
  interval.it_interval.tv_usec = 0;
  interval.it_value.tv_sec = 1;
  interval.it_value.tv_usec = 0;
  if ( setitimer(ITIMER_REAL, &interval, NULL) < 0 ) {
    perror("setitimer");
    exit(1);
  }

  /*
   * That's it, now off we go...
   */
  for ( ;; ) ;

  endwin();
}


/*
 * Handler for alarm signal, retrieve the current time, convert it
 * to a string and then display it using drawimage()
 */
void tick ( int sig ) 
{
  struct timeval theTime;

  gettimeofday(&theTime, NULL);
  drawimage(ctime(&theTime.tv_sec));
}

  
/*
 * Use this function to draw an "image", as represented by
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

