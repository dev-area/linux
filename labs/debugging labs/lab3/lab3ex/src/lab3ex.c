
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 80

char *getline(FILE *fp)
{
	int i,c;
	char buf[LEN];
	
	for(i=0 ; i< LEN ; i++)
	{
		if ( ( c=getc(fp)) == EOF || c == '\n')
			break;
		
		buf[i] = (char)c;
	}
	
	if ( c == EOF && i == 0)
		return NULL;
	
	buf[i] = '\0';
	return buf;
}

int main(int argc,char *argv[]) {

	FILE *fp;
	char filename[20];
	char *p;
	if(argc != 2)
	{
		puts("usage: lab3ex [filename]");
		exit(0);
	}
	strcpy(filename,argv[1]);
	fp=fopen(filename,"r");
	while (p = getline(fp) != NULL)
		puts(p);
	return EXIT_SUCCESS;
}
