#include <iostream>
#include "transferFunction.h"

int transferFunction::calculateFunctionValDiff(unsigned char*** passData, int passRes[3])
{
	int min = INT_MAX;
	int max = INT_MIN;
	//算function value的最大最小值
	for (int i = 0; i < passRes[0]; i++)
	{
		for (int j = 0; j < passRes[1]; j++)
		{
			for (int k = 0; k < passRes[2]; k++)
			{
				if (max < (int)passData[i][j][k])
				{
					max = (int)passData[i][j][k];
				}
				if (min > (int)passData[i][j][k])
				{
					min = (int)passData[i][j][k];
				}

			}
		}
	}
	//將最大減最小找出值域的分布並配空間給每個值
	return max - min + 1;
}

int transferFunction::calculateGradientMagDiff(unsigned char*** passData, int passRes[3])
{
	const int axis[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
	float min = INT_MAX, max = INT_MIN;
	//計算每個取樣點的gradient
	gradientMagnitude = new float [passRes[0] * passRes[1] * passRes[2]];
	gradientComponent = new float[passRes[0] * passRes[1] * passRes[2] * 4];
	for (int i = 0; i < passRes[0]; i++)
	{
		for (int j = 0; j < passRes[1]; j++)
		{
			for (int k = 0; k < passRes[2]; k++)
			{
				//v[]紀錄此點的座標index
				int v[3] = { i, j, k };
				float tempGradient[3];
				//三個維度
				for (int p = 0; p < 3; p++)
				{
					//微分法的前項、後項index
					int dim[2][3] = { 0 };
					float delta = 0.0;
					//先算好要計算該點gradient的前後項index
					for (int q = 0; q < 3; q++)
					{
						//v[q]是該計算點的第q維座標 + axis是第p維的第q維是否有貢獻(0或1)
						dim[0][q] = (int)v[q] + axis[p][q];
						dim[1][q] = (int)v[q] - axis[p][q];
						delta = 2.0;
						if ((int)v[q] + axis[p][q] >= passRes[q])
						{
							dim[0][q] = (int)v[q];
							dim[1][q] = (int)v[q] - axis[p][q];
							delta = 1.0;
						}
						if ((int)v[q] - axis[p][q] < 0)
						{
							dim[0][q] = (int)v[q] + axis[p][q];
							dim[1][q] = (int)v[q];
							delta = 1.0;
						}
					}
					tempGradient[p] = (passData[dim[0][0]][dim[0][1]][dim[0][2]] - passData[dim[1][0]][dim[1][1]][dim[1][2]]) / delta;
					gradientComponent[i * passRes[1] * passRes[2] * 4 + j * passRes[2] * 4 + k * 4 + p] = tempGradient[p];
				}
				//第四個值放function value
				gradientComponent[i * passRes[1] * passRes[2] * 4 + j * passRes[2] * 4 + k * 4 + 3] = (float)passData[i][j][k] / 255.0;
				//用三個分量算gradient magnitude
				gradientMagnitude[i * passRes[1] * passRes[2] +  j * passRes[2] + k] = sqrt(pow(tempGradient[0], 2) + pow(tempGradient[1], 2) + pow(tempGradient[2], 2));
				//向量正規化
				for (int p = 0; p < 3; p++)
				{
					gradientComponent[i * passRes[1] * passRes[2] * 4 + j * passRes[2] * 4 + k * 4 + p] /= gradientMagnitude[i * passRes[1] * passRes[2] + j * passRes[2] + k];
				}
				//計算magnitude的max, min值
				if (min > gradientMagnitude[i * passRes[1] * passRes[2] + j * passRes[2] + k])
				{
					min = gradientMagnitude[i * passRes[1] * passRes[2] + j * passRes[2] + k];
				}
				if (max < gradientMagnitude[i * passRes[1] * passRes[2] + j * passRes[2] + k])
				{
					max = gradientMagnitude[i * passRes[1] * passRes[2] + j * passRes[2] + k];
				}
			}
		}
	}
	//將最大減最小找出值域的分布並配空間給每個值(無條件進位)
	return (int)(max - min) + 1;
}

int* transferFunction::calculateFuncValCount(unsigned char*** passData, int passRes[3])
{
	//依照function value最大最小值的差分配interval數
	int* temp = new int[diffFuncValue];
	//初始化各interval count
	for (int i = 0; i < diffFuncValue; i++)
	{
		temp[i] = 0;
	}
	//計算各interval的個數
	for (int i = 0; i < passRes[0]; i++)
	{
		for (int j = 0; j < passRes[1]; j++)
		{
			for (int k = 0; k < passRes[2]; k++)
			{
				temp[(int)passData[i][j][k]]++;
			}
		}
	}
	return temp;
}

int** transferFunction::calculateFuncValGradientMagCount(unsigned char*** passData, int passRes[3])
{
	//配置2D transfer function regular grid空間及初始化
	int** temp = new int* [diffFuncValue];
	for (int i = 0; i < diffFuncValue; i++)
	{
		temp[i] = new int[diffGradientMag];
		for (int j = 0; j < diffGradientMag; j++)
		{
			temp[i][j] = 0;
		}
	}

	//計算2D transfer function的regular grid
	for (int i = 0; i < passRes[0]; i++)
	{
		for (int j = 0; j < passRes[1]; j++)
		{
			for (int k = 0; k < passRes[2]; k++)
			{
				//將原magnitude值做transfer再計算count
				float pos = gradientMagnitude[i * passRes[1] * passRes[2] + j * passRes[2] + k];
				if (pos < 1.0) { pos = 1.0; }
				else if (pos > 100.0) { pos = 100.0; }
				pos = 20 * log(pos);
				temp[(int)passData[i][j][k]][(int)round(pos)]++;
			}
		}
	}
	return temp;
}

void transferFunction::equalizeFuncValCount()
{
	int maxCountNumberFV = 0;
	for (int i = 0; i < diffFuncValue; i++)
	{
		if (funcValCount[i] > maxCountNumberFV)
		{
			maxCountNumberFV = funcValCount[i];
		}
	}
	maxCountNumberFV++;
	//cumulative distribution equalization
	//存每一grid上數字的出現次數
	int* FVVoxelCountArray = new int[maxCountNumberFV];
	int* cumulativeFVVoxelCount = new int[maxCountNumberFV];
	for (int i = 0; i < maxCountNumberFV; i++)
	{
		FVVoxelCountArray[i] = 0;
		cumulativeFVVoxelCount[i] = 0;
	}
	for (int i = 0; i < diffFuncValue; i++)
	{
		FVVoxelCountArray[funcValCount[i]]++;
	}

	cumulativeFVVoxelCount[0] = FVVoxelCountArray[0];
	for (int i = 1; i < maxCountNumberFV; i++)
	{
		cumulativeFVVoxelCount[i] = cumulativeFVVoxelCount[i - 1] + FVVoxelCountArray[i];
	}

	int maxCumulativeFV = cumulativeFVVoxelCount[maxCountNumberFV - 1];
	for (int i = 0; i < diffFuncValue; i++)
	{
		//調整每個grid中的count數
		funcValCount[i] = round(255.0 * (cumulativeFVVoxelCount[funcValCount[i]] - cumulativeFVVoxelCount[0]) / (maxCumulativeFV - cumulativeFVVoxelCount[0]));
	}
}

void transferFunction::equalizeFuncValGradientMagCount()
{
	int maxCountNumberGrid = 0;
	for (int i = 0; i < diffFuncValue; i++)
	{
		for (int j = 0; j < diffGradientMag; j++)
		{
			if (funcValGradientMagCount[i][j] > maxCountNumberGrid)
			{
				maxCountNumberGrid = funcValGradientMagCount[i][j];
			}
		}
	}
	maxCountNumberGrid++;
	//cumulative distribution equalization
	//存每一grid上數字的出現次數
	int* gridVoxelCountArray = new int[maxCountNumberGrid];
	int* cumulativeGridVoxelCount = new int[maxCountNumberGrid];
	for (int i = 0; i < maxCountNumberGrid; i++)
	{
		gridVoxelCountArray[i] = 0;
		cumulativeGridVoxelCount[i] = 0;
	}
	for (int i = 0; i < diffFuncValue; i++)
	{
		for (int j = 0; j < diffGradientMag; j++)
		{
			//if (regularGrid[i][j] >= maxCountNumber) { cout << "reg " << regularGrid[i][j] << endl; continue; }
			gridVoxelCountArray[funcValGradientMagCount[i][j]]++;
		}
	}
	cumulativeGridVoxelCount[0] = gridVoxelCountArray[0];
	for (int i = 1; i < maxCountNumberGrid; i++)
	{
		cumulativeGridVoxelCount[i] = cumulativeGridVoxelCount[i - 1] + gridVoxelCountArray[i];
	}
	int c = 0;
	int maxCumulativeGrid = cumulativeGridVoxelCount[maxCountNumberGrid - 1];
	for (int i = 0; i < diffFuncValue; i++)
	{
		for (int j = 0; j < diffGradientMag; j++)
		{
			//調整每個grid中的count數
			funcValGradientMagCount[i][j] = round(255.0 * (cumulativeGridVoxelCount[funcValGradientMagCount[i][j]] - cumulativeGridVoxelCount[0]) / (maxCumulativeGrid - cumulativeGridVoxelCount[0]));
			c++;
		}
	}
}

void transferFunction::transferFunctionSetup(unsigned char*** passData, int passRes[3])
{
	this->diffFuncValue = calculateFunctionValDiff(passData, passRes);
	this->diffGradientMag = calculateGradientMagDiff(passData, passRes);
	this->funcValCount = calculateFuncValCount(passData, passRes);
	this->funcValGradientMagCount = calculateFuncValGradientMagCount(passData, passRes);
	this->equalizeFuncValCount();
	this->equalizeFuncValGradientMagCount();
}