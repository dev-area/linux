/*
 ============================================================================
 Name        : lab4ex.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LEN 40


int main(int argc, char *argv[]) 
{
	char *p;
	char *q = "hello world and have a nice day";
	char *r;
	
	
	puts("try to alloc first memory block ");
	p=malloc(sizeof(argv[1]));
	if(p == NULL)
	{
		puts("failed to allocate first memory block");
		exit(0);
	}
	strcpy(p,argv[1]);
	
	printf("first memory block:\n");
	puts("====================");
	puts(p);
	
	puts("try to allocate second memory block ");
	r=malloc(MAX_LEN);
	if(r == NULL)
	{
		puts("failed to allocate second memory block");
		exit(0);
	}
	strcpy(r,q);
	printf("second memory block:\n");
	puts("====================");
	puts(r);
	
	puts("display in uppercase");
	puts("====================");
	while(p)
		putchar(toupper(*p++));
	printf("\n\n\n\n");
	while(r)
		putchar(toupper(*r++));
	
	free(p);
	free(r);
	return EXIT_SUCCESS;
}
