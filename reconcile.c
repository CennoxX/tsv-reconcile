#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * filename_1 = NULL;
char * rowname_1 = NULL;
char * filename_2 = NULL;
char * rowname_2 = NULL;
char * output = NULL;
char * puffer = NULL;

void loadarguments(int argi, char **argv)
{
	if (argi != 6)
	{
		printf("format: %s <input file 1> <input row 1> <input file 2> <input row 2> <output file>\n",argv[0]);
		exit(0);
	}
	
	filename_1 = (char *)malloc(sizeof(char)*strlen(argv[1])+1);
	strcpy(filename_1,argv[1]);
	rowname_1 = (char *)malloc(sizeof(char)*strlen(argv[2])+1);
	strcpy(rowname_1,argv[2]);
	filename_2 = (char *)malloc(sizeof(char)*strlen(argv[3])+1);
	strcpy(filename_2,argv[3]);
	rowname_2 = (char *)malloc(sizeof(char)*strlen(argv[4])+1);
	strcpy(rowname_2,argv[4]);
	output = (char *)malloc(sizeof(char)*strlen(argv[5])+1);
	strcpy(output,argv[5]);
}

void test_loadarguments(int argi, char **argv)
{
	filename_1 = "imdb.tsv";
	rowname_1 = "imdb";
	filename_2 = "imdb2.tsv";
	rowname_2 = "IMDb ID";
	output = "output.tsv";
}

int getrownumber(char * filename,char * rowname)
{
	int test = 0;
	int rowtoget = 0;
	char * temp = NULL;
	temp = (char *)malloc(sizeof(char)*20);
	puffer = (char *)malloc(sizeof(char)*1024);
	
	FILE * filepointer;
	filepointer = fopen(filename,"r");
	
	if (fgets(puffer,4096,filepointer)!=NULL)
	{
		char * eol = rindex(puffer,'\n');
		if (eol != NULL) *eol='\0';
		eol = rindex(puffer,'\r');
		if (eol != NULL) *eol='\0';
		
		temp = strtok(puffer, "\t");
		if(strcmp(temp,rowname) == 0)//is 0 (s1 equal to s2), <0 (s1 less than s2) or >0 (s1 greater than s2)
		{
			rowtoget=0;
		}
		else
		{
			rowtoget++;
			test = strcmp(strtok(NULL, "\t"),rowname);
			while(test != 0)
			{
				rowtoget++;
				temp = strtok(NULL, "\t");
				test = strcmp(temp,rowname);
			}
		}
	}
	free(temp);
	free(puffer);
	fclose(filepointer);
	return rowtoget;
}

void printfile(char * filename)
{
	FILE * filepointer;
	filepointer = fopen(filename,"r");
	puffer = (char *)malloc(sizeof(char)*1024);
	while (fgets(puffer,4096,filepointer)!=NULL)
	{
		char * eol = rindex(puffer,'\n');
		if (eol != NULL) *eol='\0';
		printf("%s\n",puffer);
	}
	free(puffer);
	fclose(filepointer);
}

void Free()
{
	free(filename_1);
	free(filename_2);
	free(rowname_1);
	free(rowname_2);
	free(output);
}

int main(int argi, char **argv)
{
	loadarguments(argi, argv);//comment out for testing
	//test_loadarguments(argi, argv);
	
	printf("\e[1;1H\e[2J");	
	
	int rowtoget_1 = getrownumber(filename_1, rowname_1);
	printf("index of row of %s to get: %d\n", filename_1, rowtoget_1);
	printfile(filename_1);
	
	printf("\n");	
	
	int rowtoget_2 = getrownumber(filename_2, rowname_2);
	printf("index of row of %s to get: %d\n", filename_2, rowtoget_2);
	printfile(filename_2);
	
	Free();//comment out for testing
	return 0;
}
