/* This is a example 2 of how NOT to handle dynamic memory allocation
 */
#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* If you want to turn off dynamic memory debugging:
 *
 * #define DEBUG_DYNMEM	0
 */
#include "dbm.h"
 
int main(void)
{
    char*   p;
    char*   q = "hello world";
        
    p = malloc(strlen(q + 1));      /* Should be (strlen(q) + 1) */
    strcpy(p, q);
                                    /* No more allocations are made */
                                    /* We should still detect the error */

    return 0;
}

