/*
 * test_send.c
 *
 * This test goes with test_receive.c.
 * test_send.c does a loop of mq_sends,
 * and test_receive.c does a loop of mq_receives.
 */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <mqueue.h>
#include <errno.h>

#define PMODE 0666
extern int errno;

int main()
{
int i;
int status = 0;
mqd_t mqfd;
char msg_buffer[100];
struct mq_attr attr;
int open_flags = 0;
int num_bytes_to_send;
int priority_of_msg;

printf("START OF TEST_SEND \n");

/* Fill in attributes for message queue */
attr.mq_maxmsg = 20;
attr.mq_msgsize = 100;
attr.mq_flags   = 0;

/* Set the flags for the open of the queue.
 * Make it a blocking open on the queue, meaning it will block if
 * this process tries to send to the queue and the queue is full.
 * (Absence of O_NONBLOCK flag implies that the open is blocking)
 *
 * Specify O_CREAT so that the file will get created if it does not
 * already exist.
 *
 * Specify O_WRONLY since we are only planning to write to the queue,
 * although we could specify O_RDWR also.
 */
open_flags = O_WRONLY|O_CREAT;

/* Open the queue, and create it if the receiving process hasn't
 * already created it.
 */
mqfd = mq_open("myipc",open_flags,PMODE,&attr);
if (mqfd == -1)
    {
    perror("mq_open failure from main");
    exit(0);
    };

/* Fill in a test message buffer to send */
msg_buffer[0] = 'P';
msg_buffer[1] = 'R';
msg_buffer[2] = 'I';
msg_buffer[3] = 'O';
msg_buffer[4] = 'R';
msg_buffer[5] = 'I';
msg_buffer[6] = 'T';
msg_buffer[7] = 'Y';
msg_buffer[8] = '1';
msg_buffer[9] = 'a';

num_bytes_to_send = 10;
priority_of_msg = 1;

/* Perform the send 10 times */
for (i=0; i<10; i++)
    {
    status = mq_send(mqfd,msg_buffer,num_bytes_to_send,priority_of_msg);
    if (status == -1)
        perror("mq_send failure on mqfd");
    else
        printf("successful call to mq_send, i = %d\n",i);
    }

/* Done with queue, so close it */
if (mq_close(mqfd) == -1)
    perror("mq_close failure on mqfd");

printf("About to exit the sending process after closing the queue \n");
}

