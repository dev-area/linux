/*
   react.c  Chapter 15

*/

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/times.h>
#include <time.h>

int main()
{
  struct termios oldterm, newterm;
  struct tms tbuf; clock_t start, end;
  char ch = 7;	
  int n;
  
  tcgetattr(0,&oldterm);
  newterm = oldterm;
  newterm.c_lflag &= ~(ICANON|ECHO);
  newterm.c_cc[VMIN] = 0; 	   /* Return first character read */
  newterm.c_cc[VTIME] = 50; 	/* or wait for 5 seconds */
  tcsetattr(0,TCSAFLUSH,&newterm);
  
  printf("Press any character when you hear the bell...");
  
  fflush(stdout);   
  srand(times(&tbuf));       
  sleep(rand()%7+3);
  write(1,&ch,1);   
  start = times(&tbuf);
  n = read(0,&ch,1);  
  end = times(&tbuf);        
  putchar('\n');
  tcsetattr(0,TCSAFLUSH,&oldterm);
  
  if (n == 0)
    printf("Too s l  o   w     !\n");
  else
    printf("Time: %.3f seconds\n",(double)(end-start)/CLK_TCK);

  return !n;

}
