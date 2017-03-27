/*
   msgclt.c  Chapter 10

*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#define MAXBUF    (1024)

typedef struct 
{
 	long type;            /* Message type, must be > 0 */ 
	char data[MAXBUF+1];  /* Data being transferred */ 
} msg;

#define MSG_KEY   20
#define FILENAME  60
#define DATA      50
#define ERROR     10


int main(int argc, char *argv[])
{
   int qid, length;
   msg mesg;

   key_t key = ftok(getenv("HOME"),0);

   /* Queue must exist here */
   if ((qid=msgget(key, 0)) == -1)
   {
      perror("msgget failed"); 
      exit(1); 
   }

   /* Build filename message and send it */
   strcpy (mesg.data, argv[1]);
   mesg.type = FILENAME;
   msgsnd (qid, &mesg, strlen(mesg.data), 0);

   /* Wait for replies from server, print them to stdout.
      Terminate when we get a zero length message */
   while ((length=msgrcv(qid, &mesg, MAXBUF, -50 ,0)) > 0) 
   {
      write (1, mesg.data, length);
   }

   return 0;

}
