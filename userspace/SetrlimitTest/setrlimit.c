#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

void sig(int s)
{
	puts("signal");
}
int main ()
{
	struct rlimit rl;
	signal(SIGXCPU,sig);
	/* Obtain the current limits. */
	getrlimit (RLIMIT_CPU, &rl);
	/* Set a CPU limit of 1 second. */
	rl.rlim_cur = 1;
	rl.rlim_max = 5;
	setrlimit (RLIMIT_CPU, &rl);
	/* Do busy work. */
	sleep (5);
	return 0;
}
