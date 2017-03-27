#include<stdio.h>

int fn(int *p);
int r=100;

int main()
{
	int *p = 0;
	return fn(p);
}

int fn(int *p)
{
	int y=*p;
	return y;
}
