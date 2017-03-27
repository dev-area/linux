#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
    int i;
    for (i=1; i<argc; i++)
        printf ("%2d: %s\n", i, argv[i]);
    return 0;
}
