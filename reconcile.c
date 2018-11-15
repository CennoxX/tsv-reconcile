#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * filename_1 = NULL;
char * rowname_1 = NULL;
char * filename_2 = NULL;
char * rowname_2 = NULL;
char ** ids_1 = NULL;
char ** ids_2 = NULL;
char * output = NULL;
char * puffer = NULL;
char * ids[0];
int numberoflines = 0;

void loadarguments(int argi, char **argv)
{
	if (argi != 6)
	{
		printf("format: %s <input file 1> <input row 1> <input file 2> <input row 2> <output file>\n",argv[0]);
		exit(0);
	}
	
	filename_1 = strdup(argv[1]);
	rowname_1 = strdup(argv[2]);
	filename_2 = strdup(argv[3]);
	rowname_2 = strdup(argv[4]);
	output = strdup(argv[5]);
}

void test_loadarguments(int argi, char **argv)
{
	filename_1 = strdup("imdb.tsv");
	rowname_1 = strdup("imdb");
	filename_2 = strdup("imdb2.tsv");
	rowname_2 = strdup("IMDb ID");
	output = strdup("output.tsv");
}

int getrownumber(char * filename,char * rowname)
{
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
		while(strcmp(temp,rowname) != 0)
		{
			rowtoget++;
			temp = strtok(NULL, "\t");
		}
	}
	free(puffer);
	fclose(filepointer);
	return rowtoget;
}

char ** getidarray(char * filename, int rowtoget)
{
	numberoflines = 0;
	int i = 0;
	char * temp = NULL;
	temp = (char *)malloc(sizeof(char)*20);
	FILE * filepointer;
	filepointer = fopen(filename,"r");
	puffer = (char *)malloc(sizeof(char)*1024);
	fgets(puffer,4096,filepointer);//delete header
	while (fgets(puffer,4096,filepointer)!=NULL)
	{
		char * eol = rindex(puffer,'\n');
		if (eol != NULL) *eol='\0';
		
		printf("%s\n",puffer);
		temp = strtok(puffer, "\t");
		for(i=0; i < rowtoget; i++) temp = strtok(NULL, "\t");
		ids[numberoflines++]=strdup(temp);
	}
	free(puffer);
	fclose(filepointer);
	return ids;
}

void Free()
{
	free(filename_1);
	free(filename_2);
	free(rowname_1);
	free(rowname_2);
	free(output);
	while(numberoflines > 0) free(ids[numberoflines--]);
}

int cmpstr(const void *p1, const void *p2)
{
	return strcmp(* (char * const *) p1, * (char * const *) p2);
}
	
int main(int argi, char **argv)
{
	int i = 0;
	loadarguments(argi, argv);//comment out for testing
	//test_loadarguments(argi, argv);
	printf("\e[1;1H\e[2J");	
	
	int rowtoget_1 = getrownumber(filename_1, rowname_1);
	printf("index of row from %s to get: %d\n", filename_1, rowtoget_1);
	ids_1 = getidarray(filename_1,rowtoget_1);
	printf("\nIDs from %s:\n",filename_1);
	for(i=0; i < numberoflines; i++) printf("%s\n",ids_1[i]);
	printf("\nsorted IDs:\n");
	qsort(ids_1, numberoflines, sizeof(char*), cmpstr);	
	for(i=0; i < numberoflines; i++) printf("%s\n",ids_1[i]);
	printf("\n");
	
	int rowtoget_2 = getrownumber(filename_2, rowname_2);
	printf("index of row from %s to get: %d\n", filename_1, rowtoget_1);
	ids_2 = getidarray(filename_2,rowtoget_2);

	Free();
	return 0;
}
