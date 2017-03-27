/*
 * main.c
 *
 *  Created on: May 14, 2010
 *      Author: root
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "app.h"


task all_tasks[NUM_OF_TASKS] = {
		 tcpServerTask,
		 tcpClientTask,
		 uartTask1,
		 uartTask2,
		 writeDataTask,
		 computeTask,
		 spiTask
};

int main(int argc,char *argv[])
{
	int i,pid,status,md,size;
	shared_data *data;
	size = sizeof(shared_data);

    md = shm_open("/app_memory", O_CREAT|O_RDWR, 0);

    if((ftruncate(md, size)) == -1){    /* Set the size */
        perror("ftruncate failure");
        exit(1);
    }

    data = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, md, 0);

	for( i=0 ; i<NUM_OF_TASKS ; i++ )
	{
		if( (pid=fork()) == 0)
		{
			all_tasks[i]();
			exit(0);
		}
		else
		{
			data->p_ids[i] = pid;
		}
	}
	while(1)
	{
		pid=wait(&status);
		switch (pid)
		{
		case TCPServer:
			puts("tcp server terminated");
			break;
		case TCPClient:
			puts("tcp client terminated");
			break;
		case UART1:
			puts("uart1 terminated");
			break;
		case UART2:
			puts("uart2  terminated");
			break;
		case WriteData:
			puts("writedata terminated");
			break;
		case Compute:
			puts("compute terminated");
			break;
		case SPI:
			puts("spi terminated");
			break;
		default:
			puts("error");
			break;
		}
	}

	return 0;
}
