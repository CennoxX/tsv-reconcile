#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LINELENGTH 1024
#define IDLENGTH 20

char * filename_1 = NULL;
char * rowname_1 = NULL;
char * filename_2 = NULL;
char * rowname_2 = NULL;
char * output = NULL;
char * lines[0];
int numberoflines = 0;
int idrowtoget = 0;

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


void fileExists(char *filename)
{
	FILE * filepointer;
	if(filepointer = fopen(filename, "r"))
	{
		fclose(filepointer);
	}
	else
	{
		printf("%s existiert nicht\n", filename);
		exit(0);
	}
}
int getrownumber(char * filename,char * rowname)
{
	char * puffer = NULL;
	int rowtoget = 0;
	char * temp = NULL;
	temp = (char *)malloc(sizeof(char)*IDLENGTH);
	puffer = (char *)malloc(sizeof(char)*LINELENGTH);
	
	FILE * filepointer;
	filepointer = fopen(filename,"r");
	
	if (fgets(puffer,LINELENGTH,filepointer)!=NULL)
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

char ** getlinesarray(char * filename)
{
	char * puffer = NULL;
	numberoflines = 0;
	FILE * filepointer;
	filepointer = fopen(filename,"r");
	puffer = (char *)malloc(sizeof(char)*LINELENGTH);
	fgets(puffer,LINELENGTH,filepointer);
	while (fgets(puffer,LINELENGTH,filepointer)!=NULL)
	{
		char * eol = rindex(puffer,'\n');
		if (eol != NULL) *eol='\0';
		eol = rindex(puffer,'\r');
		if (eol != NULL) *eol='\0';
		
		lines[numberoflines++]=strdup(puffer);
	}
	free(puffer);
	fclose(filepointer);
	return lines;
}

char * getidfromfile(const void *p)
{
	int i = 0;
	char * temp = NULL;
	temp = (char *)malloc(sizeof(char)*LINELENGTH);
	char * ptochange = NULL;
	ptochange = (char *)malloc(sizeof(char)*LINELENGTH);
	strcpy(ptochange,* (char * const *) p);
	temp = strtok(ptochange, "\t");
	for(i=0; i < idrowtoget; i++) temp = strtok(NULL, "\t");
	return temp;
}

int cmpids(const void *p1, const void *p2)
{
	return strcmp(getidfromfile(p1), getidfromfile(p2));
}

void Free()
{
	free(filename_1);
	free(filename_2);
	free(rowname_1);
	free(rowname_2);
	free(output);
	while(numberoflines > 0) free(lines[numberoflines--]);
}

int main(int argi, char **argv)
{
	int i = 0;
	char ** lines_1 = NULL;
	loadarguments(argi, argv);//comment out for testing
	//test_loadarguments(argi, argv);
	printf("\e[1;1H\e[2J");

	printf("+---------------------------------------------------+\n");
	printf("|                     Reconcile                     |\n");
	printf("|                                                   |\n");
	printf("|       reconcile two tab spearated value files     |\n");
	printf("|             by one shared row (e.g. imdb)         |\n");
	printf("|                                                   |\n");
	printf("+---------------------------------------------------+\n");

	fileExists(filename_1);
	int rowtoget_1 = getrownumber(filename_1, rowname_1);
	printf("index of row from %s to get: %d\n", filename_1, rowtoget_1);
	lines_1 = getlinesarray(filename_1);
	idrowtoget = rowtoget_1;
	printf("\nFile %s:\n",filename_1);
	for(i=0; i < numberoflines; i++) printf("%s\n",lines_1[i]);
	printf("\nFile sorted by IDs:\n");
	qsort(lines_1, numberoflines, sizeof(char*), cmpids);	
	for(i=0; i < numberoflines; i++) printf("%s\n",lines_1[i]);
	printf("\n");
	
	fileExists(filename_2);	
	int rowtoget_2 = getrownumber(filename_2, rowname_2);
	printf("index of row from %s to get: %d\n", filename_2, rowtoget_2);

	Free();
	return 0;
}
