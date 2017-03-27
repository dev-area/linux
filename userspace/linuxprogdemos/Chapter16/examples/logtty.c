/*
   logtty.c  Chapter 16

*/

#include <termios.h>
#include <unistd.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <stropts.h>

/* Required for ptsname prototype on linux */
#define __USE_XOPEN   
#include <stdlib.h>

#define MAXBUF 2048

#ifndef INFTIM
#define INFTIM -1
#endif

struct termios oldterm, newterm;

void ttyreset()
{
   tcsetattr(0,TCSAFLUSH,&oldterm);
}

int main()
{
   int logfd, master, slave;
   char *slavename;
   struct pollfd fds[2];
   char buffer[MAXBUF];
   int n;
   pid_t pid;

   tcgetattr(0,&oldterm);
   atexit(ttyreset);
   newterm.c_lflag &= ~(ICANON|ECHO);
   newterm.c_cc[VMIN] = 1; 
   newterm.c_cc[VTIME] = 0; 
   tcsetattr(0,TCSAFLUSH,&newterm);

   logfd = open("logfile",O_WRONLY|O_CREAT|O_TRUNC,0666);
   master = open("/dev/ptmx", O_RDWR);

   grantpt(master);
   unlockpt(master);

   if ((pid=fork()) == 0) 
   {    
      slavename = ptsname(master);
      close(master);
      slave = open(slavename, O_RDWR);  

      ioctl(slave, I_PUSH, "ttcompat");
      ioctl(slave, I_PUSH, "ptem");
      ioctl(slave, I_PUSH, "ldterm");
      dup2(slave, 0);
      dup2(slave, 1);
      dup2(slave, 2);
      close(slave); 
      close(logfd);

      tcsetattr(0,TCSAFLUSH,&oldterm);
      execlp("ksh", "ksh", NULL);
      perror ("failed to exec ksh"); 
      exit(1);
   }
   else if (pid < 0) 
   {
     perror("fork failed");
     exit(1); 
   }

   fds[0].fd = 0;
   fds[0].events = POLLIN;
   fds[1].fd = master;
   fds[1].events = POLLIN;

   for (;;) 
   {
      poll (fds, 2, INFTIM);
      if (fds[1].revents&POLLERR)
         break;
      if (fds[0].revents&POLLIN) 
      {
         n = read(0,buffer,MAXBUF);
         write (master,buffer,n);
         write (logfd,buffer,n);
      }
      if (fds[1].revents&POLLIN) 
      {
         n = read(master,buffer,MAXBUF);
         write (1,buffer,n);
         write (logfd,buffer,n);
      }
  }

  return 0;

}
