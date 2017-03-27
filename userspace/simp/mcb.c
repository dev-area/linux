#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>

#define CHECK_VALUE     '\xAA'

void    dbfree(void* addr, int line, char* name);

/* This is the fundamental data structure for the whole show. It contains:
 *
 * 1. userAddress
 *      the address returned from dbmalloc/dbcalloc/dbrealloc to the user
 * 2. end
 *      the address of the end of the block returned
 *      this is where the check byte is stored
 * 3. lineNumber
 *      filled in from __LINE__ provided by the preprocessor
 * 4. fileName
 *      filled in from __FILE__ provided by the preprocessor
 */
typedef struct  MemControl
{
	void*   userAddress;
	char*   end;
	int     lineNumber;
	char*   fileName;
} MCB;


static  int     howMany;
static  MCB*    block;

/* This is the checkChain function which is largely as it appears in the
 * notes, with the exception of the atexit() call below. This puts hooks
 * into main() so that when it finishes checkChain is called. This means
 * when the program finishes memory is checked.
 */
void    checkChain(void)
{
	int             i;
	int             bad = 0;
	static  int     beenHere;
	
	/* If this is the first we've entered the function, hook ourselves
	 * into atexit
	 */
	if(beenHere == 0) {
		atexit(checkChain);
		beenHere = 1;
	}
	
	/* For each of the MCBs ...
	 */
	for(i = 0; i < howMany; i++) {

		/* If the MCB is in use and the check value has been corrupted
		 */
		if(block[i].lineNumber > 0 && *(block[i].end) != CHECK_VALUE) {

			/* Keep a count
			 */
			bad++;
			fprintf(stderr,
			  "Dynamic Memory Allocation Error!! "
			  "%d bytes, allocated %s(%d)\n",
			      block[i].end - (char*)block[i].userAddress,
			      block[i].fileName,
			      block[i].lineNumber);
		}
	}
	if(bad > 0) {
		fprintf(stderr, "Total of %d allocation(s) corrupted. "
				"Program terminating\n", bad);

		/* Calling abort here means we do not have any problems with
		 * exit calling the atexit stuff, the atexit stuff calling
		 * checkChain, checkChain calling the atexit stuff etc etc.
		 */
		abort();
	}
}


MCB*    findBlock(void)
{
	int     i;
	MCB*    newBlock;
	
	/* Search for an unused block. Unused blocks are signified by a
	 * negative or a zero value in the lineNumber member.
	 */
	for(i = 0; i < howMany; i++)
		if(block[i].lineNumber <= 0)
			return &block[i];
			
	/* Didn't find any new blocks, so allocate one more than are already
	 * there.
	 */
	newBlock = realloc(block, sizeof(MCB) * (howMany + 1));
	
	/* If the allocation failed there is no more memory left
	 */
	if(newBlock == NULL)
		return NULL;
		
	/* The block has moved in memory
	 */
	block = newBlock;
	
	/* Zero out the new MCB
	 */
	memset(&block[howMany], '\0', sizeof(MCB));

	/* Increment howMany only now when it is safe to do so (ie after the
	 * allocation has succeeded
	 */
	return &block[howMany++];
}


void*   grabMem(size_t bytes, int line, char* name)
{
	void*   p;
	MCB*    newBlock;
	
	/* Check for corruption FIRST
	 */
	checkChain();
	
	/* Find a new block to put the data into.
	 * If we fail, there must be no memory left to allocate to the user.
	 */
	if((newBlock = findBlock()) == NULL)
		return NULL;

	/* "line" comes from __LINE__
	 */
	newBlock->lineNumber = line;
	
	/* "name" comes from __FILE__
	 */
	newBlock->fileName = name;
	
	/* Grab slightly more memory than the user asks for
	 */
	if((p = malloc(bytes + 1)) == NULL) {
		
		/* The allocation failed, mark this block as unused
		 */
		newBlock->lineNumber = 0;

		/* No dynmaic memory left, return failure to the user
		 */
		return NULL;
	}
	/* Store the address
	 */
	newBlock->userAddress = p;
	
	/* Calculate the address of the end of the block and store
	 */
	newBlock->end = ((char*)p + bytes);
	
	/* Write the check value at the end of the block
	 */
	*newBlock->end = CHECK_VALUE;
	
	/* All done, return the address
	 */
	return p;
}


/* dbmalloc is simple, grabMem already does all of the work
 */
void * dbmalloc(size_t bytes, int line, char* name)
{
	return grabMem(bytes, line, name);
}


/* dbcalloc is also straightforward
 */
void*   dbcalloc(size_t bytes, size_t number, int line, char* name)
{
	/* Could do some overflow error checking here and throw some sort
	 * of error
	 */
	size_t  total = bytes * number;
	void*   p;
	
	p = grabMem(total, line, name);

	/* Clear the memory. This is the essential difference between
	 * calloc and malloc
	 */
	if(p != NULL)
		memset(p, '\0', total);
	
	return p;
}


/* Just a simple function at the moment, this could be modified to output
 * details of the blocks of memory we have allocated. This would give 
 * useful information as to how efficiently we are using memory.
 *
 * Calling abort here avoids problems with atexit stuff.
 */
void    dberror(char* message, int line, char* name)
{
	fprintf(stderr, "%s: line %d, file %s\n", message, line, name);
	
	abort();
}


/* This is more complicated than dbmalloc or dbcalloc because we have to
 * worry about the old block.
 */
void*   dbrealloc(void* old, size_t bytes, int line, char* name)
{
	int     i;
	void*   newAddr;
	
	/* This is what the real realloc does.
	 */
	if(old == NULL)
		return dbmalloc(bytes, line, name);
		
	/* and this, too, is what the real realloc does.
	 */
	if (bytes == 0)
	{
		dbfree(old, line, name);
		return NULL;
	}
	
	checkChain();
	
	/* Check to see if the address the user is giving us is one we know
	 * about.
	 */
	for(i = 0; i < howMany; i++) {
		if(block[i].userAddress == old)
			break;
	}
	
	/* If not found, that is an error
	 */
	if(i == howMany)
		dberror("Invalid address passed to realloc", line, name);
	
	/* Just as before, grab more memory than the user requires so we may
	 * store the check byte
	 */
	if((newAddr = realloc(old, bytes + 1)) == NULL)
		
		/* No memory left
		 */
		return NULL;

	/* Remember the new addresses and insert the check value again
	 */
	block[i].userAddress = newAddr;
	block[i].end = ((char*)newAddr + bytes);
	*block[i].end = CHECK_VALUE;
	
	return newAddr;
}


/* Free a block of memory with checking
 */
void    dbfree(void* addr, int line, char* name)
{
	int     i;
	
	/* This is what the real free does, so we must do it too
	 */
	if(addr == NULL)
		return;
		
	checkChain();
	
	/* The address the user has given us must be one we know about.
	 * Scan all the MCBs until it is found
	 */
	for(i = 0; i < howMany; i++)
		if(block[i].userAddress == addr)
			break;

	if(i == howMany)
		dberror("Invalid address passed to free", line, name);

	if(block[i].lineNumber <= 0)
		dberror("Memory block freed twice", line, name);
		
	/* The address checks out ok, so free it up
	 */
	free(addr);
	
	/* Mark this block as unused
	 */
	block[i].lineNumber = -1;
}

