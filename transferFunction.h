#pragma once
#include <stdlib.h>
#include <cmath>

class transferFunction
{
public:
	void transferFunctionSetup(unsigned char*** passData, int passRes[3]);
	int diffFuncValue;
	int diffGradientMag;
	int* funcValCount;
	int** funcValGradientMagCount;
	//�s�C��voxel��gradient���C�Ӥ��q
	float* gradientComponent;
	float* gradientMagnitude;
private:
	int calculateFunctionValDiff(unsigned char*** passData, int passRes[3]);
	int calculateGradientMagDiff(unsigned char*** passData, int passRes[3]);
	int* calculateFuncValCount(unsigned char*** passData, int passRes[3]);
	int** calculateFuncValGradientMagCount(unsigned char*** passData, int passRes[3]);
	void equalizeFuncValCount();
	void equalizeFuncValGradientMagCount();
};