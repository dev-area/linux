#include<unistd.h>
#include<stdio.h>


int main(int argc ,char **argv)
{
	int i;
	for(i=0;i<argc;i++)
	{
		puts(argv[i]);
	}
	opterr = 0;
	while((i=getopt(argc,argv,"ad:l")) != EOF )
	{
		switch(i)
		{
		case 'a': puts("a flag");
				  break;
		case 'd': puts("d flag");
				  puts(optarg);
				  break;
		case 'l': puts("l flag");
				  break;
		case '?': puts("error flag");
				  break;
					  
		}
		
	}
	return 0;
}