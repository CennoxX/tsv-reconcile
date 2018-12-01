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
int idrowtoget = 0;

void printtime(){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("%d:%d:%d: ", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

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
		printtime();
		printf("%s existiert nicht oder der Zugriff auf die Datei wird verweigert.\n", filename);
		exit(0);
	}
}

int getrownumber(char * filename,char * rowname)
{
	char * puffer = NULL;
	int rowtoget = 0;
	char * temp = NULL;
	temp = (char *)malloc(sizeof(char)*LINELENGTH);
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

char *getsortedline(char * puffer, int rowtoget)
{
	char *toktemp = strtok(puffer, "\t");
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

char *removeRow(char * puffer, int rowtoget)
{
	char *toktemp = strtok(puffer, "\t");
	char *row = malloc(sizeof(char) * LINELENGTH);
	strcpy(row, "");
	int i = 0;
	while(toktemp != NULL)
	{
		if(i != rowtoget)
		{
			strcat(row, "\t");
			strcat(row, toktemp);
		}
		toktemp = strtok(NULL, "\t");
		i++;
	}
	return row;
}

char ** getlinesarray(char * filename, int *numberoflines, int rowtoget, char * list_header)
{
	char * puffer = NULL;
	char ** lines = NULL;		  
	*numberoflines = 0;
	FILE * filepointer;
	filepointer = fopen(filename,"r");
	puffer = (char *) malloc(sizeof(char)*LINELENGTH);
	fgets(list_header,LINELENGTH,filepointer);
	char * eol = rindex(list_header,'\n');
	if (eol != NULL) *eol='\0';
	eol = rindex(list_header,'\r');
	if (eol != NULL) *eol='\0';

	while (fgets(puffer,LINELENGTH,filepointer) != NULL)
	{
		char * eol = rindex(puffer,'\n');
		if (eol != NULL) *eol='\0';
		eol = rindex(puffer,'\r');
		if (eol != NULL) *eol='\0';
		*numberoflines = *numberoflines + 1;
		lines = realloc(lines, sizeof(char) * LINELENGTH * *numberoflines);//erweitert char ** immer dynamisch um ein Element
		lines[*numberoflines - 1] = getsortedline(puffer,rowtoget);
	}
	free(puffer);
	fclose(filepointer);
	return lines;
}

char * getidfromline(const void *p)
{
	char * temp = NULL;
	temp = (char *)malloc(sizeof(char)*LINELENGTH);
	strcpy(temp,* (char * const *) p);
	strtok(temp, "\t");
	//printf("%s\n",(const char*)temp);
	return temp;
}

char * getidfromlinechar(const char *p)
{
	char * temp = NULL;
	temp = (char *)malloc(sizeof(char)*LINELENGTH);
	strcpy(temp, p);
	strtok(temp, "\t");
	//printf("%s\n",(const char*)temp);
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
	int i = 0;
	*numberoflines = 0;
	char * item = NULL;
	for(i=0; i < numberoflines_2; i++)
	{
		//item = "nm1631269";//test
		item = getidfromlinechar(list_2[i]);
		item = bsearch (&item, list_1, numberoflines_1, sizeof (char*), cmpids);//search for this id
		if(item != NULL)
		{
			char *temp = malloc(sizeof(char) * 2 * LINELENGTH);
			*numberoflines = *numberoflines + 1;
			//printf("found item: '%s'\n", *(const char**)item);
			lines = realloc(lines, sizeof(char) * LINELENGTH * 2 * *numberoflines);//erweitert char ** immer dynamisch um ein Element
			strcat(temp, list_2[i]);
			strcat(temp, "\t");
			//cut imdb id before combining
			char *item2 = removeRow(*(char**)item, 0);
			strcat(temp, item2);		
			lines[*numberoflines - 1] = temp;
		}
		else 
		{
			//	printf("item could not be found\n");
		}
	}
	return lines;
}

void writetofile(char ** lines, char * filename, int numberOfLines, char * header)
{
	int i = 0;
	FILE *filepointer;
	filepointer = fopen(filename, "w");
	if(filepointer == NULL)
	{
		printf("File %s does not exists\n", filename);
		return;
	}

	fprintf(filepointer, "%s\n", header);
	for(i = 0 ; i < numberOfLines ; i++)
	{
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
	printf("|       reconcile two tab spearated value files     |\n");
	printf("|             by one shared row (e.g. imdb)         |\n");
	printf("|                                                   |\n");
	printf("+---------------------------------------------------+\n");
	
	printtime();
	printf("reading %s …\n", filename_1);
	filecheck(filename_1);
	int rowtoget_1 = getrownumber(filename_1, rowname_1);
	int numberoflines_1 = 0;
	char * list1_header = malloc(sizeof(char) * LINELENGTH);
	char ** list_1 = getlinesarray(filename_1, &numberoflines_1, rowtoget_1, list1_header);
	//printf("Header: %s\n", list1_header);
	//for(i=0; i < numberoflines_1; i++) printf("%s\n",list_1[i]);
	
	printtime();
	printf("sorting %s …\n", filename_1);
	qsort(list_1, numberoflines_1, sizeof(char*), cmplines);
	//for(i=0; i < numberoflines_1; i++) printf("%s\n",list_1[i]);
	
	printtime();
	printf("reading %s …\n", filename_2);
	filecheck(filename_2);
	int rowtoget_2 = getrownumber(filename_2, rowname_2);
	int numberoflines_2 = 0;
	char * list2_header = malloc(sizeof(char) * LINELENGTH);
	char ** list_2 = getlinesarray(filename_2, &numberoflines_2, rowtoget_2, list2_header);
	//printf("Header: %s\n", list2_header);
	//for(i = 0 ; i < numberoflines_2 ; i++) printf("%s\n", list_2[i]);
	
	printtime();
	printf("searching %s …\n", filename_1);
	int numberofcombinedlines = 0;
	char * fullHeader = getsortedline(list1_header, rowtoget_1);
	strcat(fullHeader, getsortedline(list2_header, rowtoget_2));
	//printf("FullHeader: %s\n", fullHeader);
	char ** comparedlines = comparelines(list_1, list_2, numberoflines_1, numberoflines_2, &numberofcombinedlines);

	//for(i = 0 ; i < numberofcombinedlines ; i++) printf("%s\n", comparedlines[i]);
	printtime();
	printf("Successfull ended!\n");

	printtime();
	printf("write to File %s\n", output);
	writetofile(comparedlines, output, numberofcombinedlines, fullHeader);
	free(fullHeader);

		
	Free();
	return 0;
}
