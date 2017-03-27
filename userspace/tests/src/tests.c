/*
 ============================================================================
 Name        : tests.c
 Author      : liran
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
void change()
{
	char *str="hello";
	puts(str);
	*str = 'D';
}
int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	change();
	change();
	return EXIT_SUCCESS;
}
