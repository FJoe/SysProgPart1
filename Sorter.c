#include "Sorter.h"
#include "MergeSort.c"

FILE** getcsvFiles(char* dirName){
	FILE** files = (FILE**) malloc(sizeof(FILE*) * 255);
	int* size = (int*) malloc(sizeof(int));
	*size = 0;

	DIR* dir = opendir(dirName);
	if(!dir){
		return NULL;
	}

	if(strcmp(dirName, "./") != 0){
		dirName = realloc(dirName, strlen(dirName) + strlen("/"));
		strcat(dirName, "/");
	}

	getcsvFilesHelp(files, dirName, dir, size);

	free(size);
	return files;
}

void getcsvFilesHelp(FILE** files, char* dirName, DIR* dir, int* curSize){
	if((*curSize) == 255)
		return;

	struct dirent* newDirent = readdir(dir);

	while(newDirent != NULL){
		char* base = strdup(dirName);
		base = (char*) realloc(base, strlen(base) + strlen(newDirent->d_name));
		strcat(base, newDirent->d_name);

		if(newDirent->d_type == DT_DIR && !(strcmp(newDirent->d_name, ".") == 0 || strcmp(newDirent->d_name, "..") == 0))
		{
			DIR* newDir = opendir(base);
			if(newDir != NULL && strcmp(base, "./.git") != 0)
			{
				base = (char*) realloc(base, strlen(base) + strlen("/"));
				strcat(base, "/");
				getcsvFilesHelp(files, base, newDir, curSize);
			}			
			
			
		}
		else{
			char* point = strchr(newDirent->d_name, '.');
			if(point != NULL && strcmp(point, ".csv") == 0){
				FILE* newFile  = (FILE*)malloc(sizeof(FILE));
				newFile = fopen(base, "r");
				if(newFile != NULL){
					files[*curSize] = newFile;
					(*curSize) = 1 + (*curSize);
				}
				
			}

		}
		free(base);
		newDirent = readdir(dir);
	}
	
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
	char* base = (char*) malloc(sizeof(char) * 3);
	base[0] = '.';
	base[1] = '/';
	base[2] = '\0';

	if(argc > 3 && strcmp(argv[3], "-d") == 0)
	{
		base = (char*) realloc(base, strlen(base) + strlen(argv[4]));
		strcat(base, argv[4]);
	}

	FILE** files = getcsvFiles(base);

	if(files == NULL){
		printf("ERROR: directory not found\n");
		free(base);
		return -1;
	}
	

	free(base);
	int i = 0;

	while(files[i] != NULL)
	{
		free(files[i]);
		i++;
	}
	free(files);
/**

	//Get column heading and create copy of it
	char* header = (char*) calloc(1024, sizeof(char));
	if(fgets(header, sizeof(char) * 1024, stdin) == NULL){
		printf("ERROR: file empty");
		return -1;
	}
	char* row1 = strdup(header);

	//If column to search for is found, get column number
	char* colToSort = argv[2];
	int colNumToSort = 0;
	const char delim[2] = ",";
	const char otherDelim[3] = "\"";
	char* curHead = strsep(&row1, delim);
	while( curHead != NULL && strcmp(colToSort, trimSpace(curHead)) != 0){
		curHead = strsep(&row1, delim);
		colNumToSort++;
	}
	if(curHead == NULL){
		printf("ERROR: column to sort not found");
		return -1;
	}

	//Print column headers to output file
	printf("%s", header);

	//Creating list of DataRow
	DataRow** list = (DataRow**) malloc(sizeof(DataRow*) * 20000);
	char dataType = getDataType(colToSort);
	int curRowNum = 0;
	
	char* origRow = (char*) calloc(1024, sizeof(char));
	
	//Goes through each row in csv file
	while(fgets(origRow, sizeof(char) * 1024, stdin) != NULL){
		//New dataRow for array
		DataRow* newDataRow = (DataRow*) malloc(sizeof(DataRow));
		newDataRow->dataType = dataType;

		char* rowDelim = strdup(origRow);

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

			char wordToAdd[300];  
			strcpy(wordToAdd, curWord);
			

			if(wordToAdd[0] == '"' && wordToAdd[strlen(wordToAdd) - 1] != '"')
			{
				char* otherHalf = strsep(&rowDelim, otherDelim);

				strcat(wordToAdd, ",");
				strcat(wordToAdd, otherHalf);	
				strcat(wordToAdd, "\"");		

				curWord = strsep(&rowDelim, delim);
			}
			char* trimWord = trimSpace(wordToAdd);

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

			i++;
		}
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
		printf("%s\n", list[j]->data);

		free(list[j]->dataCompare);
		free(list[j]->data);
		free(list[j]);
	}
	free(header);
	free(origRow);
	free(list);

**/

	return 0;
}

















