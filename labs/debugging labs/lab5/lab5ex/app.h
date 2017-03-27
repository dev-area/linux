/*
 * app.h
 *
 */

#ifndef APP_H_
#define APP_H_

#define NUM_OF_TASKS 		(3)
#define MAX_NAME_LEN 		(64)
#define CHILD 				(0)
#define MILI_SECOND 		(1000000) // nano seconds
#define UART_BUFFER_SIZE	(100)
#define NET_BUFFER_SIZE		(500)


typedef short int word16;
typedef unsigned char byte;

// update this if task added
typedef enum { WriteData, TCPServer , TCPClient} task_type;

// task data
typedef struct {
	void (*task)();
	char name[MAX_NAME_LEN];
}task_t;


#pragma pack(1) // no alignment


// buffer type for tcp/ip communication
typedef struct {
	int msgNum;
	byte buf[NET_BUFFER_SIZE];
	word16 len;
	word16 checksum;
}net_buffer_t;

#pragma pack() // restore default alignment


// data type for shared memory
// add fields to share more data between program tasks

typedef struct {
	int p_ids[NUM_OF_TASKS];
	char names[NUM_OF_TASKS][MAX_NAME_LEN];

}shared_data;

void tcpServerTask();
void tcpClientTask();
void writeDataTask();


short checksum(byte *addr, int count);

#endif /* APP_H_ */
