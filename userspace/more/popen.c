#include <stdio.h>


void main(void)
{
	char buf[100]="";
	FILE *fp = popen("./sc.py", "r");
	fread(buf,5,1,fp);
	puts(buf);
}
	
