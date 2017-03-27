/*
 * spiTask.c
 *
 *  Created on: May 15, 2010
 *      Author: root
 */
#include<unistd.h>
#include<stdio.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "app.h"

void spiTask()
{
	int md,size;
	shared_data *data;
	size = sizeof(shared_data);

    md = shm_open("/app_memory", O_RDWR, 0);

   
    data = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, md, 0);
    
    printf("pid=%d\n",data->p_ids[TCPServer]);
    
    prctl(PR_SET_NAME,"SPI");
	sleep(5);
	while(1);
}
