#include <stdio.h>
    #include <stdlib.h>
    #include <obstack.h>

void *mymalloc(size_t s)
{
	printf("my size=%d\n",s);
	if(s>10000)
		return NULL;
	return malloc(s);
}

void myfree(void *p)
{
	puts("myfree");
	free(p);
}

    #define obstack_chunk_alloc mymalloc
    #define obstack_chunk_free myfree


    int main(void)
    {
            struct obstack ob_stack;
            obstack_init(&ob_stack);

            double a = 5.0;
            double b = 24.0;
            double c = 2.42;

            double *pa = obstack_copy(&ob_stack, &a, sizeof a);
            double *pb = obstack_copy(&ob_stack, &b, sizeof b);
            double *pc = obstack_copy(&ob_stack, &c, sizeof c);
	    int *p=obstack_alloc(&ob_stack, 5000);

            printf("%p %p %p\n", pa, pb, pc);
	    obstack_free(&ob_stack,pa);
            printf("%f %f %f %f %f %f\n", pa[0], pa[1], pa[2], pa[3], pa[4], pa[5]);

          //  obstack_free(&ob_stack, NULL);
            return 0;
    }
