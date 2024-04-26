#include <vector>
#include <algorithm>
#include <iostream>
#include "colorMap.h"

colorMap::colorMap(float maxFV)
{
	this->size = maxFV + 1;
	this->rgba = new float* [this->size];
	for (int i = 0; i < this->size; i++)
	{
		this->rgba[i] = new float[4];
		for (int j = 0; j < 4; j++)
		{
			this->rgba[i][j] = 0.0;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		this->controlPointIndex[i].reserve(this->size);
	}

	this->functionValue = new float[this->size];
	for (int i = 0; i < this->size; i++)
	{
		this->functionValue[i] = i;
	}

	//設置default ColorMap(r,g,b,a皆等於1)
	for (int i = 0; i < 4; i++)
	{
		this->userDefineControlPoint(0, 255, i);
		this->userDefineControlPoint(255, 255, i);
	}
}

colorMap::~colorMap()
{
}

void colorMap::userDefineControlPoint(int idx, float colorValue, int rgbaType)
{
	//取樣點座標或數值超出範圍直接return
	if (idx >= this->size || idx < 0 || colorValue >= 256 || colorValue < 0)
	{
		return;
	}
	this->rgba[idx][rgbaType] = colorValue;
	//將未加入過的取樣點加入(避免重複新增)
	if (std::find(this->controlPointIndex[rgbaType].begin(), this->controlPointIndex[rgbaType].end(), idx) == this->controlPointIndex[rgbaType].end())
	{
		//新增index時，確保vector內index的排序
		auto it = std::upper_bound(this->controlPointIndex[rgbaType].begin(), this->controlPointIndex[rgbaType].end(), idx);
		this->controlPointIndex[rgbaType].insert(it, idx);
	}
	else
	{
		//std::cout << "duplicate add\n";
	}
	recalculateColorSpline(rgbaType);
}

bool colorMap::userCancelControlPoint(int idx, int rgbaType)
{
	//取樣點座標超出範圍直接return
	if (idx >= this->size || idx < 0)
	{
		return false;
	}
	//之前已加入過的點才能刪除，否則不動作
	if (std::find(this->controlPointIndex[rgbaType].begin(), this->controlPointIndex[rgbaType].end(), idx) != this->controlPointIndex[rgbaType].end())
	{
		this->controlPointIndex[rgbaType].erase(std::remove(this->controlPointIndex[rgbaType].begin(), this->controlPointIndex[rgbaType].end(), idx), this->controlPointIndex[rgbaType].end());
		this->rgba[idx][rgbaType] = 0.0;
		recalculateColorSpline(rgbaType);
		return true;
	}
	else
	{
		std::cout << "not been added\n";
		return false;
	}
}

void colorMap::recalculateColorSpline(int rgbaType)
{
	int controlPointSize = this->controlPointIndex[rgbaType].size();
	std::cout << "control point size: " << controlPointSize << std::endl;
	if (controlPointSize == 0)
	{
		for (int i = 0; i < this->size; i++)
		{
			//將所有點的值都清零
			this->rgba[i][rgbaType] = 0.0;
		}
	}
	else if (controlPointSize == 1)
	{
		for (int i = 0; i < this->size; i++)
		{
			//將其他點的值都清零
			if (i != this->controlPointIndex[rgbaType][0])
			{
				this->rgba[i][rgbaType] = 0.0;
			}
		}
	}
	else
	{
		//第一個控制點以前的值都設為0
		for (int i = 0; i < this->controlPointIndex[rgbaType][0]; i++)
		{
			this->rgba[i][rgbaType] = 0.0;
		}
		//依照有控制點的值分別內插到其他點上
		for (int i = 0; i < controlPointSize - 1; i++)
		{
			int low = this->controlPointIndex[rgbaType][i], high = this->controlPointIndex[rgbaType][i + 1];
			for (int j = low + 1; j < high; j++)
			{
				this->rgba[j][rgbaType] = this->rgba[low][rgbaType] + (this->rgba[high][rgbaType] - this->rgba[low][rgbaType]) * ((float)(j - low) / (float)(high - low));
			}
		}
		//最後一個控制點以後的值都設為0
		for (int i = this->controlPointIndex[rgbaType][controlPointSize - 1] + 1; i < this->size; i++)
		{
			this->rgba[i][rgbaType] = 0.0;
		}
	}
}