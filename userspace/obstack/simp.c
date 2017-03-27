#include<stdio.h>

int fn(int s)
{
	int arr[s];
	arr[0]=100;
	return arr[0];
}

void main(void)
{
	int i,n;
	scanf("%d",&i);
	n=fn(i);
	printf("%d\n",n);
}
