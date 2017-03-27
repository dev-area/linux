/*
   msgsrv.c  Chapter 10

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


int main()
{
   int qid, length, fd;
   msg mesg;

   key_t key=ftok(getenv("HOME"),0);

   /* Get handle to message queue, create it it's not there */
   if ((qid=msgget(key, IPC_CREAT|S_IRWXU)) == -1)
   { 
      perror("msgget failed"); 
      exit(1); 
   }

     while ((length=msgrcv(qid,&mesg,MAXBUF,FILENAME,0)) > 0) 
  {
     /* Incoming message has file name, return file contents */
     mesg.data[length] = '\0';

     if ((fd = open(mesg.data, O_RDONLY))!= -1)
     {
        /* Send each chunk of file back as a message */
        mesg.type = DATA;
        while ((length=read(fd,mesg.data,MAXBUF)) > 0)
           msgsnd(qid, &mesg, length, 0);

        close (fd);
     } 
     else 
     {
       /* Can't open file, just return error */
       strcpy (mesg.data, "could not open file\n");
       mesg.type = ERROR;
       msgsnd (qid, &mesg, strlen(mesg.data), 0);
     }

     msgsnd (qid, &mesg, 0, 0);

  }

  msgctl (qid, IPC_RMID, 0);  

  return 0;

}
