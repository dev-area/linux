#include <stdio.h>
#include <stdlib.h>

#define LIMIT 25,000

int getnumber()
{
	int num;
	puts("enter number");
	scanf("%d",num);
	return num;
}


int main(void) {
	int x,y,z;
	puts("!!!Hello World!!!"); 
	
	x = getnumber();
	y = getnumber();
	
	z= x/y;
	
	printf("result=%d\n",z);
	
	if(z > LIMIT)
	{
		puts("big");
	}
	else
	{
		puts("small");
	}
	
	
	
	return EXIT_SUCCESS;
}
