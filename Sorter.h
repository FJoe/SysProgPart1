/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef SORTER_H
#define SORTER_H

//data to compare each row by. Either comparing data of string or numeric values
union data_compare{
	double numData;
	char* stringData;
};
typedef union data_compare DataCompare;


//single row of csv file. Has content of the csv file, data to compare each row with, and the datatype of the data to compare with
struct data_row{

/**
	dataType:
		n = numData
		s = stringData
**/
	char dataType;
	char* data;
	DataCompare* dataCompare;	
};
typedef struct data_row DataRow;

//Gets pointer to array of file pointers of type csv files. Also traverses through any directories inside dir
void sortcsvFiles(char* dir, char* outputDir, char* colToSort, int* counter);

//Helper method to getcsvFiles
void sortcsvFilesHelp(char* dirName, DIR* dir, char* outputDir, char* colToSort, int* counter);

//Gets col number of string in file header (first row of file)
int getColNum(char* fileDir, char* colToSort);

//sort csv file
void sort(char* fileName, char* outDir, char* colToSort);

//Trims leading and trailing spaces from a string
char* trimSpace(char* str);

//Gets dataType of column name
char getDataType(char* data);

//Suggestion: prototype a mergesort function
void merge(DataRow ** array, int left, int mid, int right);
void mergeSort(DataRow ** array, int left, int right);



#endif
