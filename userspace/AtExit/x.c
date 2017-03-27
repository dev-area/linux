#include<stdio.h>
#include<stdlib.h>

void handler1()
{
	puts("exit1");
}

void handler2()
{
	puts("exit2");
}
 
int main()
{
	atexit(handler1);
	atexit(handler2);
	puts("hello world!!!");
	return 1;
}
