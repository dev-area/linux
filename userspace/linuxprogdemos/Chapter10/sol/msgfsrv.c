#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/*
 * Simple file service, using message queues as IPC.
 * Server can communicate with many clients simultaneously.
 */

#include "msg.h"

int qid;

int main(void)
{
    int clntid;
    int len;
    int fd;
    struct message mesg;
    struct sigaction act;
    sigset_t mask;
    void shutdown();
    key_t key=ftok(getenv("HOME"),0);

    /*
     * Get hold of the message queue
     */
    if ((qid=msgget(key, IPC_CREAT|S_IRWXU)) < 0) { 
	perror("msgget failed"); 
	exit(1); 
    }

    /*
     * Set up signal handlers so server cleans up after itself
     */
    sigemptyset(&mask);
    act.sa_handler = shutdown;
    act.sa_mask = mask;
    act.sa_flags = 0;
    if ( sigaction(SIGINT, &act, NULL) < 0 ) {
	perror("Installing handler for INT");
	exit(1);
    }
    if ( sigaction(SIGQUIT, &act, NULL) < 0 ) {
	perror("Installing handler for QUIT");
	exit(1);
    }
    if ( sigaction(SIGTERM, &act, NULL) < 0 ) {
	perror("Installing handler for TERM");
	exit(1);
    }

    /*
     * Main server loop, wait for a message to appear on the queue
     * with a type field of "FILENAME" (1).
     */
    while ( (len = msgrcv(qid,(struct msgbuf*)&mesg,MAXBUF,FILENAME,0)) > 0) {

	/*
	 * The "len" value returned above includes the "subtype" field
	 * of the message structure. The actual data value we want is 
	 * sizeof(long) bytes shorter than this.
	 */
	len -= sizeof(long);

	/*
	 * The request from the client carries the filename in the data
	 * field, so make sure that it is null-terminated. The pid of the
	 * sender is in the subtype field. We need this so we can send
	 * replies back to the correct client.
	 */
        mesg.m_data[len] = '\0';
	clntid = mesg.m_subtype;
	printf("Request: file %s for client %d\n", mesg.m_data, clntid);

	/*
	 * Open the file to read
	 */
        fd = open(mesg.m_data, O_RDONLY);

        if (fd >= 0) {

	    /*
	     * Send its contents back to the client.
	     * Reply messages have the client pid as the main type,
	     * the subtype indicates if this is data or an error message.
	     */
            mesg.m_subtype = DATA;
	    mesg.m_type = clntid;
            while ( (len = read (fd, mesg.m_data, MAXBUF)) > 0 ) {

		/*
		 * Make sure that we include the subtype field in the length
		 * value of the message we send back, otherwise the client will
		 * lose sizeof(long) bytes from each message
		 */
                if (msgsnd(qid, (struct msgbuf*)&mesg, len+sizeof(long), 0) < 0) {
		    perror("server msgsnd failed"); 
		    exit(1); 
		}
            }
            close (fd);
        } else {

	    /*
	     * Send the error message back to the client.
	     */
            strcpy(mesg.m_data, strerror(errno));
            mesg.m_subtype = ERROR;
	    mesg.m_type = clntid;
            if (msgsnd(qid, (struct msgbuf*)&mesg,
		       strlen(mesg.m_data)+sizeof(long), 0) < 0 ) {
		perror("server msgsnd failed"); 
		exit(1); 
	    }
        }

	/*
	 * Send an empty message to the client to indicate end-of-message
	 */
	if ( msgsnd(qid,(struct msgbuf*)&mesg,0,0) < 0 ) {
	    perror("Closing msgsnd failed\n");
	    exit(1);
	}
    }

    if (len < 0) 
        perror ("server msgrcv failed");

    shutdown(0);
}


/*
 * Tidy up - remove the msgq
 */
void shutdown ( int signo ) {

    if (msgctl (qid, IPC_RMID, 0) < 0) {
        perror ("shutdowm: msgctl remove failed");
        exit(1);
    }

    exit(0);

}
