/*
 * msgq.h by Kamran Karimi
 * Header file for hello1.c and hello2.c
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#define MSG_KEY 40
#define MSG_MODE (IPC_EXCL | 0777)
/* use of 'IPC_DIPC' is the ONLY thing that makes 
 * this program a distributed * one. Everything 
 * else is normal System V IPC programming */
#define MSG_TYPE 10
#define MSG_SIZE 512
struct message
{
   long mtype;
   char mtext[MSG_SIZE];
};
