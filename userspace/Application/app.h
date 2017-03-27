/*
 * app.h
 *
 *  Created on: May 14, 2010
 *      Author: root
 */

#ifndef APP_H_
#define APP_H_

#define NUM_OF_TASKS 7

typedef void (*task)();



typedef struct {
	int p_ids[NUM_OF_TASKS];
	char buf[100];
	int num1;
	pthread_mutex_t mx;

}shared_data;

void tcpServerTask();
void tcpClientTask();
void uartTask1();
void uartTask2();
void writeDataTask();
void computeTask();
void spiTask();

typedef enum { TCPServer , TCPClient, UART1, UART2, WriteData, Compute, SPI } task_type;

#endif /* APP_H_ */
