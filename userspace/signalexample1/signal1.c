#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIG_STOP_CHILD SIGUSR1 

int main()
{
pid_t pid;
sigset_t newmask, oldmask;

    if ((pid = fork()) == 0) {    /*Child*/
        struct sigaction action; 
        void catchit();

        sigemptyset(&newmask); 
        sigaddset(&newmask, SIG_STOP_CHILD); 
        sigaddset(&newmask, SIGUSR2); 
        sigaddset(&newmask, SIGCHLD); 
                
        sigprocmask(SIG_BLOCK, &newmask, &oldmask); 

        action.sa_flags = 0; 
        action.sa_handler = catchit;

        if (sigaction(SIG_STOP_CHILD, &action, NULL) == -1) { 
            perror("sigusr: sigaction");
            _exit(1);
        }
        sigsuspend(&oldmask); 
        
    }
    else {                             /* Parent */
        int stat;
        sleep(1); 
        kill(pid, SIG_STOP_CHILD); 
        pid = wait(&stat); 
        printf("Child exit status = %d\n", WEXITSTATUS(stat));
     _exit(0);
     }
     return 0;
}
void catchit(int signo) 
{
       printf("Signal %d received from parent\n", signo);
       _exit(0);
}

