#include "Sorter.h"
#include "MergeSort.c"
#include <unistd.h>
#include <sys/mman.h>

char getDataType(char* data){
	if(strcmp(data, "color") == 0)
		return 's';
 	else if(strcmp(data, "director_name") == 0)
		return 's'; 
	else if(strcmp(data, "actor_2_name") == 0)
		return 's';
	else if(strcmp(data, "genres") == 0)
		return 's';
	else if(strcmp(data, "actor_1_name") == 0)
		return 's';
	else if(strcmp(data, "movie_title") == 0)
		return 's';
	else if(strcmp(data, "actor_3_name") == 0)
		return 's';
	else if(strcmp(data, "plot_keywords") == 0)
		return 's';
	else if(strcmp(data, "movie_imdb_link") == 0)
		return 's';
	else if(strcmp(data, "language") == 0)
		return 's';
	else if(strcmp(data, "country") == 0)
		return 's';
	else if(strcmp(data, "content_rating") == 0)
		return 's';

	if(strcmp(data, "num_critic_for_reviews") == 0)
		return 'n';
	else if(strcmp(data, "duration") == 0)
		return 'n'; 
	else if(strcmp(data, "director_facebook_likes") == 0)
		return 'n'; 
	else if(strcmp(data, "actor_3_facebook_likes") == 0)
		return 'n';
	else if(strcmp(data, "actor_1_facebook_likes") == 0)
		return 'n';
	else if(strcmp(data, "gross") == 0)
		return 'n';
	else if(strcmp(data, "num_voted_users") == 0)
		return 'n';
	else if(strcmp(data, "cast_total_facebook_likes") == 0)
		return 'n';
	else if(strcmp(data, "facenumber_in_poster") == 0)
		return 'n';
	else if(strcmp(data, "num_user_for_reviews") == 0)
		return 'n';
	else if(strcmp(data, "budget") == 0)
		return 'n';
	else if(strcmp(data, "title_year") == 0)
		return 'n';
	else if(strcmp(data, "actor_2_facebook_likes") == 0)
		return 'n';
	else if(strcmp(data, "imdb_score") == 0)
		return 'n';
	else if(strcmp(data, "aspect_ratio") == 0)
		return 'n';
	else if(strcmp(data, "movie_facebook_likes") == 0)
		return 'n';

	return 'e';	
}

char* trimSpace(char* str){
	int end = strlen(str) - 1;
	while(str[end] == ' ' || str[end] == '\n' || str[end] == '\r')
	{
		str[end] = '\0';
		end--;
	}
	while(*str == ' ')
	{
		str++;
	}
	return str;
}


int getColNum(char* fileDir, char* colToSort){
	FILE* file = fopen(fileDir, "r");

	if(!file){
		return -1;
	}

	const char delim[2] = ",";
	char* row1p = (char*) calloc(1024, sizeof(char));
	char* row1 = row1p;
	
	if(fgets(row1, sizeof(char) * 1024, file) == NULL){
		free(row1p);
		fclose(file);
		return -1;
	}
	
	int colNumToSort = 0;
	char* curHead = strsep(&row1, delim);
	while( curHead != NULL && strcmp(colToSort, trimSpace(curHead)) != 0){
		curHead = strsep(&row1, delim);
		colNumToSort++;
	}
	free(row1p);
	if(curHead == NULL){

		fclose(file);
		return -1;
	}
	return colNumToSort;
} 

void getcsvFilesHelp(char* dirName, DIR* dir, char* outDir, char* colToSort, int* counter){
	//If 255 csv files/directories were found
	if((*counter) == 255)
		return;

	//Gets first file in directory dir
	struct dirent* newDirent = readdir(dir);

	//For every file in director dir
	while(newDirent != NULL){
		//Gets file into correct format for 
		char* base = strdup(dirName);
		base = (char*) realloc(base, strlen(base) + strlen(newDirent->d_name));
		strcat(base, newDirent->d_name);

		//If file is already "-sorted" with correct parameter
		char* fileName = strdup(newDirent->d_name);
		const char delim[2] = "-";

		char* sortedBy = NULL;
		char* nextSep;
		int prevDash = 0;
		int first = 1;

		while((nextSep = strsep(&fileName, delim)) != NULL)
		{
			if(first)
				first = 0;
			else{
				if(prevDash){
					sortedBy = nextSep;
					prevDash = 0;
				}
				if(strcmp(nextSep, "sorted") == 0)
					prevDash = 1;
				else
					prevDash = 0;
			}
		}
		if(sortedBy != NULL)
			sortedBy[strlen(sortedBy) - 4] = '\0';

		//If file is already sorted by correct parameter
		if(sortedBy != NULL && strcmp(sortedBy, colToSort) == 0){



		}
		//If file is a directory
		else if(newDirent->d_type == DT_DIR && !(strcmp(newDirent->d_name, ".") == 0 || strcmp(newDirent->d_name, "..") == 0))
		{
			DIR* newDir = opendir(base);
			//If new directory is found and not .git (too many directories inside)
			if(newDir != NULL && strcmp(base, "./.git") != 0)
			{
				base = (char*) realloc(base, strlen(base) + strlen("/"));
				strcat(base, "/");
					
				pid_t pidDir = fork();

				//Child process sorts csv files in new directory
				if(pidDir == 0){
					//printf("%d, ", (int)getpid());
					getcsvFilesHelp(base, newDir, outDir, colToSort, counter);	
					_exit(0);
				}
				//Parent process continues sorting csv files in current directory
				else{
					(*counter)++;

					printf("%d, ", (int)pidDir);
					fflush(stdout);	
				} 
				
			}			
		}
		//If file is not a directory
		else{
			char* point = strchr(newDirent->d_name, '.');

			//If file is a csv file with correct column name
			if(point != NULL && strcmp(point, ".csv") == 0 && getColNum(base, colToSort) != -1)
			{
				pid_t pidFile = fork();
				//Child process sorts file
				if(pidFile == 0){					
					//printf("%d, ", (int)getpid());
					//printf("here\n");
					if(outDir == NULL){
						sort(base, dirName, colToSort);
					}
					else{
						sort(base, outDir, colToSort);
					}
					
					_exit(0);
				}
				//Parent process continues sorting csv files in current directory
				else{
					(*counter)++;				 
					printf("%d, ", (int)pidFile);
					fflush(stdout);
				}		
			}

		}
		free(base);
		newDirent = readdir(dir); 
	} 
	while(wait(NULL) > 0){}		
}


void sortcsvFiles(char* dirName, char* outputDir, char* colToSort, int * counter){
	DIR* dir = opendir(dirName);
	if(!dir){
		return;
	}

	getcsvFilesHelp(dirName, dir, outputDir, colToSort, counter);
	return;
}

void sort(char* fileDir, char* outDir, char* colToSort){
	//If column to search for is found, get column number
	int colNumToSort = getColNum(fileDir, colToSort);
	if(colNumToSort == -1)
		return;

	FILE* file = fopen(fileDir, "r");
	//Get column heading and create copy of it
	char* header = (char*) calloc(1024, sizeof(char));
	if(fgets(header, sizeof(char) * 1024, file) == NULL){
		return;
	}

	const char delim[2] = ",";
	const char otherDelim[3] = "\"";
	const char slashDelim[2] = "/";
	char* nextSep;
	char* fileName;
	char* origFile = strdup(fileDir);
	while((nextSep = strsep(&origFile, slashDelim)) != NULL)
	{
		fileName = nextSep;
	}
	if(fileName == NULL)
		fileName = fileDir;

	fileName[strlen(fileName) - 4] = '\0';

	char* outputFile = strdup(outDir);

	outputFile = realloc(outputFile, strlen(outputFile) + strlen(fileName) + strlen("-sorted-") + strlen(colToSort) + strlen(".csv"));
	strcat(outputFile, fileName);
	strcat(outputFile, "-sorted-");
	strcat(outputFile, colToSort);
	strcat(outputFile, ".csv");
	
	FILE* outfp;
	outfp = fopen(outputFile, "w");

	//Print column headers to output file
	fprintf(outfp, header);

	//Creating list of DataRow
	DataRow** list = (DataRow**) malloc(sizeof(DataRow*) * 20000);
	char dataType = getDataType(colToSort);
	int curRowNum = 0;
	
	char* origRow = (char*) calloc(1024, sizeof(char));
	
	//Goes through each row in csv file
	while(fgets(origRow, sizeof(char) * 1024, file) != NULL){
		//New dataRow for array
		DataRow* newDataRow = (DataRow*) malloc(sizeof(DataRow));
		newDataRow->dataType = dataType;

		char* origRowDelim = strdup(origRow);
		char* rowDelim = origRowDelim;

		//newRow is data for new row with trimmed spaces
		char* newRow = (char*) calloc(1024, sizeof(char));

		//Goes through each word, trimming spaces and finding data to sort by and add data to array
		char* curWord = strsep(&rowDelim, delim);
		int firstWord = 1;
		int i = 0;

		while(curWord != NULL){
			if(firstWord)
				firstWord = 0;
			else
				strcat(newRow, ",");

			char* wordToAdd = strdup(curWord);
			if(strlen(wordToAdd) > 1 && wordToAdd[0] == '"' && wordToAdd[strlen(wordToAdd) - 1] != '"')
			{
				char* otherHalf = strsep(&rowDelim, otherDelim);

				wordToAdd = (char*)realloc(wordToAdd, strlen(wordToAdd) + strlen(",") + strlen(otherHalf) + strlen("\""));
				strcat(wordToAdd, ",");
				strcat(wordToAdd, otherHalf);	
				strcat(wordToAdd, "\"");		

				curWord = strsep(&rowDelim, delim);


			}
			char* trimOrig = strdup(wordToAdd);
			char* trimWord = trimSpace(trimOrig);

			if(i == colNumToSort){
				DataCompare* newDataCompare = (DataCompare*) malloc(sizeof(DataCompare));
				if(dataType == 'n'){
					newDataCompare->numData = atof(trimWord);
				}
				else if(dataType == 's'){
					newDataCompare->stringData = trimWord;
				}
				newDataRow->dataCompare = newDataCompare;
			}

			strcat(newRow, trimWord);
			curWord = strsep(&rowDelim, delim);

			if(wordToAdd[0] != '"')
				free(wordToAdd);

			i++;
		}
		free(origRowDelim);

		strcat(newRow, "\n");
		newDataRow->data = newRow;
		list[curRowNum] = newDataRow;
		curRowNum++;
	}

	//Sort list
	mergeSort(list, 0, (curRowNum -1));

	//Prints list then frees it
	int j;	
	for(j = 0; j < curRowNum; j++)
	{
		fprintf(outfp, list[j]->data);

		//free(list[j]->dataCompare);
		free(list[j]->data);
		free(list[j]);
	}
	free(header);
	free(origRow);
	free(list);

	fclose(file);
	fclose(outfp);
}

int main(int argc, char* argv[])
{
	//Check if valid input
	if(argc < 2){
		printf("ERROR: no arguments\n");
		return -1;
	}
	if(strcmp(argv[1], "-c") != 0){
		printf("ERROR: not sorting by columns\n");
		return -1;
	}
	if(argc < 3){
		printf("ERROR: column to sort by not given\n");
		return -1;
	}
	if(argc > 3){
		if(strcmp(argv[3], "-d") != 0 && strcmp(argv[3], "-o") != 0){
			printf("ERROR: input/output directory command not given\n");
			return -1;
		}

		if(argc == 4 || argc == 6){
			printf("ERROR: input/output directory not given\n");
			return -1;
		}
		if(argc == 7 && strcmp(argv[5], "-o") != 0){
			printf("ERROR: output directory command not given\n");
			return -1;
		}
	}
	char* colToSort = argv[2];

	char* inDir = NULL;

	//Checks if given -d folder. If none then gets current directory
	if(argc > 3 && strcmp(argv[3], "-d") == 0)
	{
		//Given is relative path
		if(strlen(argv[4]) > 1 && argv[4][0] == '.' && argv[4][1] == '/'){
			inDir = strdup(argv[4]);\
		}
		//Given is absolute path
		else if(strlen(argv[4]) > 0 && argv[4][0] == '/'){
			inDir = strdup(argv[4]);
		}
		//Given is assumed to be relative path
		else{
			inDir = (char*) malloc(sizeof(char) * 3);
			inDir[0] = '.';
			inDir[1] = '/';
			inDir[2] = '\0'; 

			inDir = (char*) realloc(inDir, strlen(inDir) + strlen(argv[4]));
			strcat(inDir, argv[4]);
		}
		if(inDir[strlen(inDir) - 1] != '/'){
			inDir = (char*) realloc(inDir, strlen(inDir) + strlen("/"));
			strcat(inDir, "/");
		}

	}
	else{
		inDir = (char*) malloc(sizeof(char) * 3);
		inDir[0] = '.';
		inDir[1] = '/';
		inDir[2] = '\0'; 
	}

	//Checks if -o directory. If none then returns null
	char* outDir = NULL;
	if((argc > 3 && argc < 6 && strcmp(argv[3], "-o") == 0) || (argc > 5 && strcmp(argv[5], "-o") == 0)){	
		char* output = NULL;
		if(argc < 6){
			output = strdup(argv[4]);
		}
		else{
			output = strdup(argv[6]);
		}

		//Given is relative path
		if(strlen(output) > 1 && output[0] == '.' && output[1] == '/'){
			outDir = strdup(output);\
		}
		//Given is absolute path
		else if(strlen(output) > 0 && output[0] == '/'){
			outDir = strdup(output);
		}
		//Given is assumed to be relative path
		else{
			outDir = (char*) malloc(sizeof(char) * 3);
			outDir[0] = '.';
			outDir[1] = '/';
			outDir[2] = '\0'; 

			outDir = (char*) realloc(outDir, strlen(outDir) + strlen(output));
			strcat(outDir, output);
		}
		if(outDir[strlen(outDir) - 1] != '/'){
			outDir = (char*) realloc(outDir, strlen(outDir) + strlen("/"));
			strcat(outDir, "/");
		}
	}

	//printf("Initial PID: %d Current dir: %s\n", (int)getpid(), inDir);
	printf("Initial PID: %d\n", (int)getpid());
	printf("PIDS of all child processes: ");

	int * counter = (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	//current process counts as 1
	*counter = 1;

	sortcsvFiles(inDir, outDir, colToSort, counter);

	printf("\nTotal number of processes: %d\n", *counter); 
	//free(counter);


	return 0;
}

















