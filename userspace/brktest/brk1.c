#define _GNU_SOURCE
#include <unistd.h>

int main(void) {
    /* Get the first address beyond the end of the heap. */
    void *b = sbrk(0);
    int *p = (int *)b;
    printf("val=%lx\n",p);
//    b=sbrk(0);
//    p=(int *)b;
//    printf("val=%x\n",p);
    /* Segfault because it is outside of the heap. */
    *p = 1;
    return 0;
}
