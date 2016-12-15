
/* 
	submitted By: hara12
	Assignment task 1 Display the contents of ASCII file, Print number of lines and number of characters. 
*/
#include "include.h"
int main(int argc, char *argv[])
{
	if (argc<2)
	{	
		printf("Usage: <Filename>\n");
		return 0;	
	}
	if(strcmp(argv[1], "-h")==0)
	{
		printf("Usage: <filename.txt>\n");
		return 0;	
	}	

 char file;
int numChar=0;
int numLine=0;

FILE *ifile= fopen(argv[1],"r");

if (ifile)
{

	while((file=fgetc(ifile)) != EOF)
	{
		printf("%c", file);
		numChar++;
		if(file == '\n') {numLine++;}
	}

printf("\nNumber of lines in text file: %d", numLine);
printf("\nNumber of Characters in text file: %d" , numChar);
printf("\n");
fclose(ifile);
}

return 0;	
}
