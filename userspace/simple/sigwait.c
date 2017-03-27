/*
   sigwait.c  Chapter 6

*/

#include <stdio.h>
#include <signal.h>

int main ()
{
    sigset_t newset, pending;

    sigemptyset (&newset);
    sigaddset (&newset,SIGINT);
    sigaddset (&newset,SIGQUIT);
    sigprocmask (SIG_BLOCK,&newset,NULL);

    printf ("Press ^C or ^\\ in the next 5 seconds\n");
    sleep(5);

    sigpending (&pending);

    if (sigismember(&pending,SIGQUIT)) 
        printf ("SIGQUIT is pending");

    if (sigismember(&pending,SIGINT)) 
        printf ("SIGINT is pending");

    return 0;

}
