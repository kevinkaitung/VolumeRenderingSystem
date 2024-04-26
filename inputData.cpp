#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

unsigned char*** readFile(const char* filenam, int res[3])
{
	unsigned char*** myData = NULL;
	//input file pointer
	FILE* fptr;
	fptr = fopen(filenam, "rb");
	if (fptr == NULL)
	{
		printf("No such file\n");
		return NULL;
	}
	long int c = 0;
	myData = (unsigned char***)malloc(res[0] * sizeof(unsigned char**));
	for (int i = 0; i < res[0]; i++)
	{
		myData[i] = (unsigned char**)malloc(res[1] * sizeof(unsigned char*));
		for (int j = 0; j < res[1]; j++)
		{
			myData[i][j] = (unsigned char*)malloc(res[2] * sizeof(unsigned char));
			//move file pointer to specific position
			fseek(fptr, c * res[2], SEEK_SET);
			//read data from the position and store to myData[i][j]
			fread(myData[i][j], sizeof(unsigned char), res[2], fptr);
			c++;
		}
	}
	fclose(fptr);
	return myData;
}
