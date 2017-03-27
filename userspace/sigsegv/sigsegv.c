#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdlib.h>
#include <ucontext.h>

void safe_func(void)
{
    puts("Safe now ?");
    exit(0); //can't return to main, it's where the segfault occured.
}

void
handler (int cause, siginfo_t * info, void *uap)
{
ucontext_t *context = uap;  
//For test. Never ever call stdio functions in a signal handler otherwise*/
  printf ("SIGSEGV raised at address %p\n", context->uc_mcontext.gregs[31]);
  
  /*On my particular system, compiled with gcc -O2, the offending instruction
  generated for "*f = 16;" is 6 bytes. Lets try to set the instruction
  pointer to the next instruction (general register 14 is EIP, on linux x86) */
  //context->uc_mcontext.gregs[REG_RIP] += 6; 
  //alternativly, try to jump to a "safe place"
  context->uc_mcontext.gregs[REG_RIP] = (unsigned int)safe_func;
}

int
main (int argc, char *argv[])
{
  struct sigaction sa;int num=80;
  sa.sa_sigaction = handler;
  int *f = NULL;
  sigemptyset (&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;
  if (sigaction (SIGSEGV, &sa, 0)) {
      perror ("sigaction");
      exit(1);
  }
  //cause a segfault
  f+=num;
  *f = 16; 
  puts("Still Alive");
  return 0;
}

