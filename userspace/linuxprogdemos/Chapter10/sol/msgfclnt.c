#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/*
 * File service client based on message queues.
 */
#include "msg.h"

int main(int argc, char *argv[])
{
    int qid;
    int len = 0;
    int mypid = getpid();
    struct message mesg;
    key_t key;

    /*
     * Argument check - make sure we are given a file to ask for...
     */
    if ( argc < 2 ) {
	fprintf(stderr, "Usage: %s file\n", argv[0]);
	exit(1);
    }

    /*
     * Get message queue - assume server has created it.
     */
    key = ftok(getenv("HOME"),0);
    if ( (qid = msgget(key, 0)) < 0) { 
	perror("msgget failed"); 
	exit(1); 
    }
    
    /*
     * Build message with type of FILENAME (1) and subtype of our pid
     */
    strcpy (mesg.m_data, argv[1]);
    mesg.m_type = FILENAME;
    mesg.m_subtype = (long)mypid;

    /*
     * Send message to server, make sure to include size of subtype in the
     * length field or else server sees a bogus filename.
     */
    if ( msgsnd( qid, (struct msgbuf*)&mesg,
		 strlen(mesg.m_data)+sizeof(long), 0) < 0) { 
	perror("client msgsnd failed"); 
	exit(1); 
    }

    /* 
     * Receive back (hopefully) file contents. Just dump it all to stdout.
     */
    while ( (len = msgrcv(qid, (struct msgbuf*)&mesg, 
			  MAXBUF+sizeof(long), mypid, 0)) > 0) {

	/*
	 * len returned here includes the subtype field, so adjust to get
	 * the correct size of the "data" portion of the message.
	 */
	len -= sizeof(long);
	if ( mesg.m_subtype == DATA )
	    write (1, mesg.m_data, len);
	else 
	    printf("Server reports error: %s\n", mesg.m_data);
    }
   
    if (len < 0) { 
	perror("client msgrcv failed"); 
	exit(1); 
    }

    exit(0);
}
