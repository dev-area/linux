/*
 * main.c
 *
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "app.h"


task_t all_tasks[NUM_OF_TASKS] = {
		 { writeDataTask,	"WriteData"		},
		 { tcpServerTask,	"TCPServer"		},
		 { tcpClientTask,	"TCPClient"		},
};

/* find the process by pid */
static int findPid(shared_data *data,int pid)
{
	int i;
	for( i=0 ; i<NUM_OF_TASKS ; i++ )
	{
		if (data->p_ids[i] == pid)
			return i;
	}
	return -1;
}

int main(int argc,char *argv[])
{
	int i,pid,status,md,size;
	shared_data *data;
	size = sizeof(shared_data);

	// parent process creates the shared memory
    md = shm_open("/app_memory", O_CREAT|O_RDWR, 0);

    if((ftruncate(md, size)) == -1){    /* Set the size */
        perror("ftruncate failure");
        exit(1);
    }

    // map to the shared memory - ready to use
    data = mmap(0, size, PROT_WRITE, MAP_SHARED, md, 0);

    // loop to create the child
	for( i=0 ; i<NUM_OF_TASKS ; i++ )
	{
		if( (pid=fork()) == CHILD)
		{
			all_tasks[i].task(); // run the main task loop
			exit(0);			 // child process task returned - terminate it
		}
		else // parent
		{
			data->p_ids[i] = pid;
			strcpy(data->names[i],all_tasks[i].name);
		}
	}
	while(1)
	{
		pid=wait(&status); // wait for any child process to quit
		switch (findPid(data,pid))
		{
		case TCPServer:
			printf("tcp server terminated pid=%d\n",pid);
			break;
		case TCPClient:
			printf("tcp client terminated pid=%d\n",pid);
			break;
		case WriteData:
			printf("writedata terminated pid=%d\n",pid);
			break;
		default:
			printf("error pid=%d\n",pid);
			exit(0);
		}
	}

	return 0;
}
