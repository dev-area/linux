/*
 * msgq2.c by Kamran Karimi
 * This program sends a message to hello1 process.
 * You should first run hello1, and then hello2 on
 * a machine in the same cluster
 */
#include "msgq.h"
int main()
{
   int msgid;
   struct message mess;
   /* gain access to the message queue that was
    * created by hello1 */
   msgid = msgget(MSG_KEY, MSG_MODE);
   if(msgid < 0) 
   {
      fprintf(stderr,"Hello2: msgget() failed BECAUSE %s\n",strerror(errno));
      exit(20);
   }
   mess.mtype = MSG_TYPE; /* not used here */
   strcpy(mess.mtext,"Hello, Distributed Programming!");
   /* now send the message. This will traverse 
    * the network if hello1 and hello2 programs 
    * are in different computers and DIPC is 
    * properly installed */
   if(msgsnd(msgid, (struct msgbuf *)&mess, sizeof(mess.mtext), 0) < 0)
   {
      fprintf(stderr,"Hello2: msgsnd() failed BECAUSE %s\n", strerror(errno));
      exit(20);
   }
   exit(0);
}