#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>

#include "definition.h"
#include "graph.h"
#include "inputData.h"
#include "transferFunction.h"

int resolution[3] = { 256, 256, 256 };
const char* fileName = "C:/Users/KevinKai/Downloads/VolumeData/Scalar/bonsai.raw";
int isoValue = 45;

using namespace std;

transferFunction tf;
unsigned char*** myData = NULL;
int main(int argc, char** argv)
{
	float*** gradientMagnitude = NULL;
	//fetch data
	myData = readFile(fileName, resolution);
	tf.transferFunctionSetup(myData, resolution);
	//cout << "max function value: " << maxFuncValue << " min function value: " << minFuncValue << " diff function value: " << diffFuncValue << endl;
	//印出資料
	//for (int i = 0; i < diffFuncValue; i++)
	{
		//cout << voxelInFuncValueRange[i] << endl;
	}
	//cout << "min gradient magnitude: " << minGradientMag << " max gradient magnitude: " << maxGradientMag << " diff gradient magnitude: " << diffGradientMag << endl;

	/*int maxCountNumber = 0;
	for (int i = 0; i < diffFuncValue; i++)
	{
		for (int j = 0; j < diffGradientMag; j++)
		{
			if (i == 0 && j == 0) { cout << " regular grid " << regularGrid[i][j] << endl; continue; }
			if (regularGrid[i][j] > maxCountNumber)
			{
				maxCountNumber = regularGrid[i][j];
			}
		}
	}

	maxCountNumber++;
	cout << "max fv " << maxCountNumber << endl;

	//cumulative distribution equalization
	int gridVoxelTotalCount = 0;
	int* cumulativeVoxelCount = new int[maxCountNumber];
	for (int i = 0; i < maxCountNumber; i++)
	{
		cumulativeVoxelCount[i] = 0;
	}
	for (int k = 0; k < maxCountNumber; k++)
	{
		for (int i = 0; i < diffFuncValue; i++)
		{
			for (int j = 0; j < diffGradientMag; j++)
			{
				//if (regularGrid[i][j] >= maxCountNumber) { cout << "reg " << regularGrid[i][j] << endl; continue; }
				if (regularGrid[i][j] == k)
				{
					gridVoxelTotalCount++;
				}
			}
			cumulativeVoxelCount[k] += gridVoxelTotalCount;
		}
	}

	int c = 0;
	int maxCumulative = cumulativeVoxelCount[maxCountNumber - 1];
	for (int i = 0; i < diffFuncValue; i++)
	{
		for (int j = 0; j < diffGradientMag; j++)
		{
			if (regularGrid[i][j] >= maxCountNumber) continue;
			//調整每個grid中的count數
			regularGrid[i][j] = round(255.0 * (cumulativeVoxelCount[regularGrid[i][j]] - cumulativeVoxelCount[0]) / (maxCumulative - cumulativeVoxelCount[0]));
			//cout << (cumulativeVoxelCount[c] - cumulativeVoxelCount[0])/(maxCumulative - cumulativeVoxelCount[0]) << endl;
			c++;
		}
	}*/
	char* tmp1 = new char[71];
	char* tmp2 = new char[71];
	strcpy(tmp1, "C:/Users/KevinKai/Desktop/科學視算/00757128_HW02/Debug/VertShader.vert");
	strcpy(tmp2, "C:/Users/KevinKai/Desktop/科學視算/00757128_HW02/Debug/FragShader.frag");
	mainSetupAndLoop(argc, argv, tmp1, tmp2, resolution);
	return 0;
}

//free memory
/*for (int i = 0; i < resolution[0]; i++)
{
	for (int j = 0; j < resolution[1]; j++)
	{
		delete[] myData[i][j];
	}
	delete[] myData[i];
}
delete myData;*/
