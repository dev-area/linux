/*
 ============================================================================
 Name        : hello.c
 Author      : liran
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>


int main(void) {
	int r=90;
	int *p=NULL;
	r++;
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	r+=10;
	*p=90;
	printf("res=%d\n",r);
	return EXIT_SUCCESS;
}
