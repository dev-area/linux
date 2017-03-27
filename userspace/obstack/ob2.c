#include <obstack.h>

#include <stdlib.h>

/* Example code listing for using obstacks */

/* Used for obstack macros (xmalloc is
   a malloc function that exits if memory
   is exhausted */

#define obstack_chunk_alloc malloc

#define obstack_chunk_free free

/* Pools */

/* Only permanent allocations should go in this pool */

struct obstack *global_pool;

/* This pool is for per-connection data */

struct obstack *connection_pool;

/* This pool is for per-request data */

struct obstack *request_pool;

void allocation_failed()

{

	exit(1);

}

int main()

{

	/* Initialize Pools */

	global_pool = (struct obstack *)

		malloc (sizeof (struct obstack));

	obstack_init(global_pool);

	connection_pool = (struct obstack *)

		malloc (sizeof (struct obstack));

	obstack_init(connection_pool);

	request_pool = (struct obstack *)

		malloc (sizeof (struct obstack));

	obstack_init(request_pool);

	/* Set the error handling function */

	obstack_alloc_failed_handler = &allocation_failed;

	/* Server main loop */

	while(1)

	{
		int r=5;
		//wait_for_connection();
		sleep(1);
		/* We are in a connection */

		while(r--)

		{

			/* Handle request */

			handle_request();

			/* Free all of the memory allocated

			 * in the request pool

			 */

			obstack_free(request_pool, NULL);

		}

		/* We're finished with the connection, time

		 * to free that pool

		 */

		obstack_free(connection_pool, NULL);

	}

}

int handle_request()

{

	/* Be sure that all object allocations are allocated
	 * from the request pool
	 */
	puts("req");
	int bytes_i_need = 400;

	void *data1 = obstack_alloc(request_pool, bytes_i_need);

	/* Do stuff to process the request */

	/* return */

	return 0;

}
