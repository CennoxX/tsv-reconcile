#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define LINELENGTH 1024

char * filename_1 = NULL;
char * rowname_1 = NULL;
char * filename_2 = NULL;
char * rowname_2 = NULL;
char * output = NULL;

void clear(){
	#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
		system("clear");
	#endif
	
	#if defined(_WIN32) || defined(_WIN64)
		system("cls");
	#endif
}

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

void test_loadarguments()
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
	char * buffer = NULL;
	int rowtoget = 0;
	char * temp = NULL;
	temp = (char *)malloc(sizeof(char)*LINELENGTH);
	buffer = (char *)malloc(sizeof(char)*LINELENGTH);
	
	FILE * filepointer;
	filepointer = fopen(filename,"r");
	
	if (fgets(buffer,LINELENGTH,filepointer)!=NULL)
	{
		char * eol = rindex(buffer,'\n');
		if (eol != NULL) *eol='\0';
		eol = rindex(buffer,'\r');
		if (eol != NULL) *eol='\0';
		
		temp = strtok(buffer, "\t");
		while(strcmp(temp,rowname) != 0)
		{
			rowtoget++;
			temp = strtok(NULL, "\t");
		}
	}
	free(buffer);
	fclose(filepointer);
	return rowtoget;
}

char *getsortedline(char * buffer, int rowtoget)
{
	char *toktemp = strtok(buffer, "\t");
	char *row = malloc(sizeof(char) * LINELENGTH);
	strcpy(row, "");
	char *imdbId = malloc(sizeof(char) * LINELENGTH);
	int i = 0;
	while(toktemp != NULL)
	{
		if(i == rowtoget)
		{
			strcpy(imdbId, toktemp);
		}
		else 
		{
			strcat(row, "\t");
			strcat(row, toktemp);
		}
		toktemp = strtok(NULL, "\t");
		i++;
	}
	strcat(imdbId, row);
	return imdbId;
}

char ** getlinesarray(char * filename, int numberoflines, int rowtoget)
{
	char * buffer = NULL;
	char ** lines = NULL;
	lines = realloc(lines, sizeof(char) * LINELENGTH * numberoflines);	
	FILE * filepointer;
	filepointer = fopen(filename,"r");
	buffer = (char *) malloc(sizeof(char)*LINELENGTH);
	fgets(buffer,LINELENGTH,filepointer);
	int i = 0;
	while (fgets(buffer,LINELENGTH,filepointer) != NULL)
	{
		if((numberoflines/100) != 0 && 0 == i % (numberoflines/100))
		{
			printf("\rreading %s %d%%", filename, i/(numberoflines/100));	
		}
		char * eol = rindex(buffer,'\n');
		if (eol != NULL) *eol='\0';
		eol = rindex(buffer,'\r');
		if (eol != NULL) *eol='\0';
		lines[i] = getsortedline(buffer,rowtoget);
		i++;
	}
	free(buffer);
	fclose(filepointer);
	return lines;
}

int getnumberoflines(char * filename)
{
	char * buffer = NULL;	  
	int numberoflines = 0;
	FILE * filepointer;
	filepointer = fopen(filename,"r");
	buffer = (char *) malloc(sizeof(char)*LINELENGTH);
	fgets(buffer,LINELENGTH,filepointer);
	while (fgets(buffer,LINELENGTH,filepointer) != NULL)
	{
		numberoflines++;
	}
	free(buffer);
	fclose(filepointer);
	return numberoflines;
}

char * getidfromline(const void *p)
{
	char * temp = NULL;
	temp = (char *)malloc(sizeof(char)*LINELENGTH);
	strcpy(temp,* (char * const *) p);
	strtok(temp, "\t");
	return temp;
}

char * getidfromlinechar(const char *p)
{
	char * temp = NULL;
	temp = (char *)malloc(sizeof(char)*LINELENGTH);
	strcpy(temp, p);
	strtok(temp, "\t");
	return temp;
}

int cmpids(const void *a, const void *b)
{
	return strcmp(getidfromline(a), getidfromline(b));
}

int cmplines(const void *a, const void *b)
{
	return strcmp(*(const char**)a, *(const char**)b);
}

void Free()
{
	free(filename_1);
	free(filename_2);
	free(rowname_1);
	free(rowname_2);
	free(output);
}

char ** comparelines(char **list_1, char** list_2, int numberoflines_1, int numberoflines_2, int *numberoflines)
{
	char ** lines = NULL;
	lines = realloc(lines, sizeof(char) * LINELENGTH * 2 * numberoflines_2);
	int i = 0;
	*numberoflines = 0;
	char * item = NULL;
	for(i=0; i < numberoflines_2; i++)
	{
		if((numberoflines_2/100) != 0 && 0 == i % (numberoflines_2/100))
		{
			printf("\rsearching %s %d%%", filename_2, i/(numberoflines_2/100));	
		}
		item = getidfromlinechar(list_2[i]);
		item = bsearch (&item, list_1, numberoflines_1, sizeof (char*), cmpids);
		if(item != NULL)
		{
			char *temp = malloc(sizeof(char) * 2 * LINELENGTH);
			*numberoflines = *numberoflines + 1;
			strcat(temp, list_2[i]);
			strcat(temp, "\t");
            char *buffer = strdup(*(char**)item);
            char *item2 = strtok(buffer, "\t");
            item2 = strtok(NULL, "");
			strcat(temp, item2);
			lines[*numberoflines - 1] = temp;
		}
	}
	return lines;
}

void writetofile(char ** lines, char * filename, int numberoflines)
{
	int i = 0;
	FILE *filepointer;
	filepointer = fopen(filename, "w");
	if(filepointer == NULL)
	{
		printf("File %s does not exist or you do not have read access.\n", filename);
		return;
	}

	for(i = 0 ; i < numberoflines ; i++)
	{
		if((numberoflines/100) != 0 && 0 == i % (numberoflines/100))
		{
			printf("\rwriting %s %d%%", output, i/(numberoflines/100));	
		}
		fprintf(filepointer, "%s\n", lines[i]);
	}

	fclose(filepointer);
}
int main(int argi, char **argv)
{
	int i = 0;
	if(strcmp(argv[1],"--test") == 0) test_loadarguments();
	else loadarguments(argi, argv);
	
	clear();
	printf("+---------------------------------------------------+\n");
	printf("|                     Reconcile                     |\n");
	printf("|                                                   |\n");
	printf("|      reconcile two tab spearated value files      |\n");
	printf("|          by one shared row (e.g. imdb id)         |\n");
	printf("|                                                   |\n");
	printf("+---------------------------------------------------+\n");
	
	//read file 1
	filecheck(filename_1);
	int rowtoget_1 = getrownumber(filename_1, rowname_1);
	int numberoflines_1 = getnumberoflines(filename_1);
	char ** list_1 = getlinesarray(filename_1, numberoflines_1, rowtoget_1);
	//for(i=0; i < numberoflines_1; i++) printf("%s\n",list_1[i]);
	
	//sort file 1
	printf("\nsorting %s …", filename_1);
	qsort(list_1, numberoflines_1, sizeof(char*), cmplines);
	//for(i=0; i < numberoflines_1; i++) printf("%s\n",list_1[i]);
	printf("\rsorting %s 100%%\n", filename_1);
	
	//read file 2
	filecheck(filename_2);
	int rowtoget_2 = getrownumber(filename_2, rowname_2);
	int numberoflines_2 = getnumberoflines(filename_2);
	char ** list_2 = getlinesarray(filename_2, numberoflines_2, rowtoget_2);
	//for(i = 0 ; i < numberoflines_2 ; i++) printf("%s\n", list_2[i]);
	
	//search in file 1
	printf("\n");
	int numberofcombinedlines = 0;
	char ** comparedlines = comparelines(list_1, list_2, numberoflines_1, numberoflines_2, &numberofcombinedlines);
	//for(i = 0 ; i < numberofcombinedlines ; i++) printf("%s\n", comparedlines[i]);
	
	//write output
	printf("\n");
	writetofile(comparedlines, output, numberofcombinedlines);
		
	Free();
	printf("\nReconciling successfull!\n");
	return 0;
}
