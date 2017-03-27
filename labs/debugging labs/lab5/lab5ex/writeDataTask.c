/*
 * writeDataTask.c
 *
 *  Created on: May 15, 2010
 *      Author: root
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <time.h>

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "app.h"

#define FILESIZE 			(32768)  // buffer size is 1Mbit = 32KB * sizeof(int)
#define NUM_OF_BLOCKS		(160)	 // 2GB = 16000 * FILESIZE * sizeof(int)
#define NUM_OF_FILES		(10)

// fill buffer with test data
void setup(int *buf,int size)
{
	int i;
	for(i=0;i<size;i++)
		buf[i]=i % 1000;
}

void writeFile(char *filename, int *buffer)
{
	int fd,i,x;
	struct timespec t1;
	t1.tv_nsec =  100 * MILI_SECOND; // 100ms
	t1.tv_sec = 0;
	fd = open(filename,O_WRONLY|O_CREAT);
	if(-1==fd)
	{
		perror("file open:");
		exit(0);
	}
	for(i=0;i<NUM_OF_BLOCKS;i++)
	{
		nanosleep(&t1,NULL);
		x=write(fd,buffer,FILESIZE * sizeof(int));
		if(x==-1)
		{
			perror("write failed");
			exit(0);
		}
	}
	close(fd);

}
void writeDataTask()
{
	int i;
	char filebuf[255] = {0};

	int testbuf[FILESIZE];
	puts("writeDataTask started");
	prctl(PR_SET_NAME,"WriteData");
	setup(testbuf,FILESIZE);
	// create the files
	for(i=0;i<NUM_OF_FILES;i++)
	{
		sprintf(filebuf,"/opt/testfiles/a%d",i);
		writeFile(filebuf,testbuf);
	}
}
