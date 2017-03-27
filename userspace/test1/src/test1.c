/*
 ============================================================================
 Name        : test1.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int x=9,y=3,z;
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	z=x+y;
	
	y+=10;
	
	x-=2;
	z*=x+y;
	
	printf("res=%d\n",z);
	return EXIT_SUCCESS;
}
