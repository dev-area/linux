#ifndef	DEBUG_DYNMEM
#define	DEBUG_DYNMEM	1
#endif

#if	DEBUG_DYNMEM

#include <stdlib.h>	/* For size_t, it SHOULD be <sys/types.h> !!! */

/* First replace these functions with our own
 */
#define	malloc(bytes)	  dbmalloc  (bytes,	__LINE__, __FILE__)
#define	calloc(n, bytes)  dbcalloc  (n, bytes,	__LINE__, __FILE__)
#define realloc(p, bytes) dbrealloc (p, bytes,	__LINE__, __FILE__)
#define	free(p)           dbfree    (p,		__LINE__, __FILE__)

/* Then provide prototypes for our own
 */
void*	dbmalloc(size_t, int, char*);
void*	dbcalloc(size_t, size_t, int, char*);
void*	dbrealloc(void*, size_t, int, char*);
void	dbfree(void*, int, char*);

#endif	/* DEBUG_DYNMEM */

