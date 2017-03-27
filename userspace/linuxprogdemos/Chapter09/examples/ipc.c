/*
   ipc.c  Chapter 9

*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

int main()
{
  struct msqid_ds buf;
  const char *home=getenv("HOME");
  key_t key=ftok(home,0);

  int id = msgget(key,IPC_CREAT|S_IRWXU);
  if (id == -1)
  { 
     perror ("IPC message get"); 
     exit(1); 
  }

  if (msgctl(id, IPC_STAT, &buf) == -1)
  { 
     perror ("IPC message STAT control"); 
     exit(1); 
  }
  
  buf.msg_perm.mode &= ~(S_IWGRP|S_IRWXO); 
  if (msgctl(id, IPC_SET, &buf) == -1)
  { 
     perror ("IPC message SET control"); 
     exit(1); 
  }
 
  return 0;

}
