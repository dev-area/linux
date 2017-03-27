/* This is a example 1 of how NOT to handle dynamic memory allocation
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
    char*   r;
	
    p = malloc(strlen(q + 1));      /* Should be (strlen(q) + 1) */
    strcpy(p, q);
    r = malloc(strlen(q) + 1);      /* Should fail here */

    return 0;
}

