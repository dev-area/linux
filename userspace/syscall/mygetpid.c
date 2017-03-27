#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
 
int mygetpid()
{
    int res;
    asm volatile ("movl $20,%%eax;int $0x80; movl %%eax,%0;":
            "=r"(res):  : "eax");
    return res;
}
 
int main(void) {
    int z;
    printf("using getpid()=%d\n",getpid());
    z=mygetpid();
    printf("res=%d\n",z);
    return EXIT_SUCCESS;
}
