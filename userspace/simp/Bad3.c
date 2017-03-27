/* This is a example 3 of how NOT to handle dynamic memory allocation
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
	
    p = malloc(strlen(q) + 1);                            /* Now is ok */
    strcpy(p, q);
    p++;
    free(p);                /* This is incorrect, since free'd address */
                            /* is NOT the same as that returned by     */
                            /* malloc                                  */

	return 0;
}

