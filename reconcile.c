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

void filecheck(char *filename)
{
	FILE * filepointer;
	if(filepointer = fopen(filename, "r"))
	{
		fclose(filepointer);
	}
	else
	{
		printf("%s existiert nicht oder der Zugriff auf die Datei wird verweigert.\n", filename);
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

char ** getlinesarray(char * filename, int *numberoflines, int sourcerow)
{
	char * puffer = NULL;
	char ** lines = NULL;		  
	*numberoflines = 0;
	FILE * filepointer;
	filepointer = fopen(filename,"r");
	puffer = (char *) malloc(sizeof(char)*LINELENGTH);
	fgets(puffer,LINELENGTH,filepointer);//exclude the first line

	while (fgets(puffer,LINELENGTH,filepointer) != NULL)
	{
		char * eol = rindex(puffer,'\n');
		if (eol != NULL) *eol='\0';
		eol = rindex(puffer,'\r');
		if (eol != NULL) *eol='\0';
		*numberoflines = *numberoflines + 1;
		lines = realloc(lines, sizeof(char) * LINELENGTH * *numberoflines);//erweitert char ** immer dynamisch um ein Element
		char * tokTemp = strtok(puffer, "\t");
		int i = 0;
		char *row = malloc(sizeof(char) * LINELENGTH);
		strcpy(row, "");
		char *imdbId = malloc(sizeof(char) * LINELENGTH);
		while(tokTemp != NULL)
		{
			if(i == sourcerow)
			{
				strcpy(imdbId, tokTemp);
			}
			else 
			{
				strcat(row, tokTemp);
				strcat(row, "\t");
			}
			tokTemp = strtok(NULL, "\t");
			i++;
		}
		strcat(imdbId, "\t");
		strcat(imdbId, row);
		lines[*numberoflines - 1] = strdup(imdbId);
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
}

int main(int argi, char **argv)
{
	int i = 0;
	if(strcmp(strdup(argv[1]),"--test") == 0) test_loadarguments(argi, argv);
	else loadarguments(argi, argv);

	printf("\e[1;1H\e[2J");//clear terminal
	printf("+---------------------------------------------------+\n");
	printf("|                     Reconcile                     |\n");
	printf("|                                                   |\n");
	printf("|       reconcile two tab spearated value files     |\n");
	printf("|             by one shared row (e.g. imdb)         |\n");
	printf("|                                                   |\n");
	printf("+---------------------------------------------------+\n");

	filecheck(filename_1);
	int rowtoget_1 = getrownumber(filename_1, rowname_1);
	printf("index of row from %s to get: %d\n", filename_1, rowtoget_1);
	int numberoflines_1 = 0;
	char ** list_1 = getlinesarray(filename_1, &numberoflines_1, rowtoget_1);
	idrowtoget = rowtoget_1;
	printf("\nIDs from %s:\n", filename_1);

	for(i=0; i < numberoflines_1; i++)
	{
		printf("%s\n",list_1[i]);
	}

	filecheck(filename_2);
	int rowtoget_2 = getrownumber(filename_2, rowname_2);
	printf("index of row %s to get: %d\n", filename_2, rowtoget_2);
	int numberoflines_2 = 0;
	char ** list_2 = getlinesarray(filename_2, &numberoflines_2, rowtoget_2);
	idrowtoget = rowtoget_2;
	printf("\nIDs from %s\n", filename_2);

	for(i = 0 ; i < numberoflines_2 ; i++)
	{
		printf("%s\n", list_2[i]);
	}
	Free();
	return 0;
}
