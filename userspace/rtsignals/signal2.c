#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIG_STOP_CHILD SIGRTMIN+1 

main()
{
pid_t pid;
sigset_t newmask, oldmask;

    if ((pid = fork()) == 0) {    /*Child*/
        struct sigaction action;
        void catchit();

        sigemptyset(&newmask);
        sigaddset(&newmask, SIG_STOP_CHILD);
        sigprocmask(SIG_BLOCK, &newmask, &oldmask);

        action.sa_flags = SA_SIGINFO; 
        action.sa_sigaction = catchit;

        if (sigaction(SIG_STOP_CHILD, &action, NULL) == -1) { 
            perror("sigusr: sigaction");
            _exit(1);
        }
        sigsuspend(&oldmask);
    }
    else {                             /* Parent */
        union sigval sval; 
        sval.sival_int = 1;
        int stat;
        sleep(1); 
        sigqueue(pid, SIG_STOP_CHILD, sval); 
        pid = wait(&stat); 
        printf("Child exit status = %d\n", WEXITSTATUS(stat));
        _exit(0);
    }
}
void catchit(int signo, siginfo_t *info, void *extra) 
{
       void *ptr_val = info->si_value.sival_ptr;
       int int_val = info->si_value.sival_int;
       printf("Signal %d, value %d  received from parent\n", signo, int_val);
       _exit(0);
}

