/*
 * mgsq1.c by Kamran Karimi
 * It initializes a message structure and waits 
 * to receive a message.
 * After running hello1, you should run hello2 on 
 * a remote machine.
 */
#include "msgq.h"
int main()
{
   int msgid;
   struct message mess;
 
   /* create the message queue. The other 
		* process will access it later */
   msgid = msgget(MSG_KEY, MSG_MODE | IPC_CREAT );
   
   if(msgid < 0) 
   {
      fprintf(stderr,"Hello1: msgget() failed BECAUSE %s\n", strerror(errno));
      exit(20);
   }
   fprintf(stderr,"Hello1: waiting to receive a message.\n");
   
   if(msgrcv(msgid, (struct msgbuf *)&mess, sizeof(mess.mtext), 0, 0) < 0)
      fprintf(stderr,"Hello1: msgrcv() failed BECAUSE %s\n", strerror(errno));
   else 
      fprintf(stderr,"Hello1: Received '%s'\n",mess.mtext); 
	  
   msgctl(msgid,IPC_RMID,NULL); 
   exit(0);
}
