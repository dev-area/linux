/*
   password.c  Chapter 15

*/

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
  char buffer[81], ch;
  struct termios oldterm, newterm;
  int fd = open("/dev/tty", O_RDWR, 0);
  FILE *fp = fdopen(fd,"w+");

  /* Read and keep a copy of the old attributes */
  tcgetattr(fd,&oldterm);
  newterm = oldterm;
  newterm.c_lflag &= ~ECHO;
  tcsetattr(fd,TCSAFLUSH,&newterm);
  fprintf(fp, "Enter password? "); 
  fflush(fp);
  fscanf(fp, "%80s", buffer); fputc('\n',fp);

  /* Now restore the previous values */
  tcsetattr(fd,TCSAFLUSH,&oldterm);
  printf ("Password = <%s>\n", buffer);

  return 0;

}
