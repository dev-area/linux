#include<stdio.h>
#include<string.h>


char *p="hello\0world";

int main()
{
	char buf[7];
	strncpy(buf,p,12);
	
	return 0;
}
