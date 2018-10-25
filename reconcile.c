#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int main(int argi, char **argv)
{
	char * file_1 = NULL;
	char * row_1 = NULL;
	char * file_2 = NULL;
	char * row_2 = NULL;
	char * output = NULL;
	char * puffer = (char *)malloc(sizeof(char)*1024);
	char * temp = NULL;
	
	int test = 0;
	int rowtoget_1 = 0;
	int rowtoget_2 = 0;

//	if (argi != 6)
//	{
//		printf("format %s <input file 1> <input row 1> <input file 2> <input row 2> <output file>\n",argv[0]);
//		return 255;
//	}
//	file_1 = (char *)malloc(sizeof(char)*strlen(argv[1])+1);
//	row_1 = (char *)malloc(sizeof(char)*strlen(argv[2])+1);
//	file_2 = (char *)malloc(sizeof(char)*strlen(argv[3])+1);
//	row_2 = (char *)malloc(sizeof(char)*strlen(argv[4])+1);
//	output = (char *)malloc(sizeof(char)*strlen(argv[5])+1);
//	
//	strcpy(file_1,argv[1]);
//	strcpy(row_1,argv[2]);
//	strcpy(file_2,argv[3]);
//	strcpy(row_2,argv[4]);
//	strcpy(output,argv[5]);


	file_1 = (char *)malloc(sizeof(char)*10);
	row_1 = (char *)malloc(sizeof(char)*10);
	file_2 = (char *)malloc(sizeof(char)*10);
	row_2 = (char *)malloc(sizeof(char)*10);
	output = (char *)malloc(sizeof(char)*10);
	temp = (char *)malloc(sizeof(char)*10);
	
	strcpy(file_1,"imdb.tsv");
	strcpy(row_1,"imdb");
	strcpy(file_2,"imdb2.tsv");
	strcpy(row_2,"IMDb ID");
	
	FILE * filepointer_1;
	FILE * filepointer_2;

	filepointer_1 = fopen(file_1,"r");
	filepointer_2 = fopen(file_2,"r");
	printf("\e[1;1H\e[2J");
	printf("---start_%s---\n", file_1);
	/* get first line */
	if (fgets(puffer,4096,filepointer_1)!=NULL)
	{
		char * eol = strsep(puffer,'\n','\r');//use strsep instead? man strtok: by one of bytes in delim
		if (eol!=NULL) *eol='\0';
		eol = rindex(puffer,'\r');
		if (eol!=NULL) *eol='\0';
		
		output = strtok(puffer, "\t");
		if(strcmp(output,row_1)==0)//is 0 (s1 equal to s2), <0 (s1 less than s2) or >0 (s1 greater than s2)
		{
			rowtoget_1=0;
		}
		else
		{
			rowtoget_1++;
			test = strcmp(strtok(NULL, "\t"),row_1);
			while(test!=0)
			{
				rowtoget_1++;
				temp = strtok(NULL, "\t");
				test = strcmp(temp,row_1);
			}
		}
	}
	
	/* get other lines */
	while (fgets(puffer,4096,filepointer_1)!=NULL)
	{
		char * eol = rindex(puffer,'\n');
		if (eol!=NULL) *eol='\0';
		printf("%s\n",puffer);
	}
	printf("---end_%s---\n", file_1);
	printf("index of row to get: %d\n\n", rowtoget_1);
	
//	do same for file_2 and row_2
	
	fclose(filepointer_1);
	fclose(filepointer_2);
	
	free (puffer);
	free(file_1);
	free(row_1);
	free(file_2);
	free(row_2);
	free(output);
	
	return 0;
}
