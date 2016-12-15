/* 
	submitted By: hara12
	Assignment task 2 Read the contents of ASCII file, Filter and write the matched to a new file.
	tested with regular expression '[[:digit:]]{2}-[[:digit:]]{2}-[[:digit:]]{3}-[[:digit:]]{4}' PhoneNumber(46-72-763-9047) 
*/

#include "include.h"
int main(int argc, char *argv[])
{
	if (argc<4)
	{	
		printf("Usage: Sourcefile Pattern Destinationfile\n");
		return 0;	
	}
	if(strcmp(argv[1], "-h")==0)
	{
		printf("Usage: Sourcefile Pattern Destinationfile\n");
		return 0;	
	}
	
	char *pattern= argv[2];
	FILE *source=fopen(argv[1],"r");
	FILE *dest=fopen(argv[3],"w");
	regex_t regEx;
	char t[512];
		//compile regular expression
		regcomp(&regEx, pattern, REG_EXTENDED);

		while (fgets(t,sizeof t, source) !=NULL)
		{// execute regular expression
			if (regexec (&regEx, t, 0, NULL, 0)==0)
			 fprintf (dest,"%s",t);
		}
fclose(source);
fclose(dest); 
return 0;
}
