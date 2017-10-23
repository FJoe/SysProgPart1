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

//Trims leading and trailing spaces from a string
char* trimSpace(char* str);

//Gets dataType of column name
char getDataType(char* data);

//Suggestion: prototype a mergesort function
void merge(DataRow ** array, int left, int mid, int right);
void mergeSort(DataRow ** array, int left, int right);



#endif
