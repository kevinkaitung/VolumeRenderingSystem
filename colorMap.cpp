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

	//�]�mdefault ColorMap(r,g,b,a�ҵ���1)
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
	//�����I�y�ЩμƭȶW�X�d�򪽱�return
	if (idx >= this->size || idx < 0 || colorValue >= 256 || colorValue < 0)
	{
		return;
	}
	this->rgba[idx][rgbaType] = colorValue;
	//�N���[�J�L�������I�[�J(�קK���Ʒs�W)
	if (std::find(this->controlPointIndex[rgbaType].begin(), this->controlPointIndex[rgbaType].end(), idx) == this->controlPointIndex[rgbaType].end())
	{
		//�s�Windex�ɡA�T�Ovector��index���Ƨ�
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
	//�����I�y�жW�X�d�򪽱�return
	if (idx >= this->size || idx < 0)
	{
		return false;
	}
	//���e�w�[�J�L���I�~��R���A�_�h���ʧ@
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
			//�N�Ҧ��I���ȳ��M�s
			this->rgba[i][rgbaType] = 0.0;
		}
	}
	else if (controlPointSize == 1)
	{
		for (int i = 0; i < this->size; i++)
		{
			//�N��L�I���ȳ��M�s
			if (i != this->controlPointIndex[rgbaType][0])
			{
				this->rgba[i][rgbaType] = 0.0;
			}
		}
	}
	else
	{
		//�Ĥ@�ӱ����I�H�e���ȳ��]��0
		for (int i = 0; i < this->controlPointIndex[rgbaType][0]; i++)
		{
			this->rgba[i][rgbaType] = 0.0;
		}
		//�̷Ӧ������I���Ȥ��O�������L�I�W
		for (int i = 0; i < controlPointSize - 1; i++)
		{
			int low = this->controlPointIndex[rgbaType][i], high = this->controlPointIndex[rgbaType][i + 1];
			for (int j = low + 1; j < high; j++)
			{
				this->rgba[j][rgbaType] = this->rgba[low][rgbaType] + (this->rgba[high][rgbaType] - this->rgba[low][rgbaType]) * ((float)(j - low) / (float)(high - low));
			}
		}
		//�̫�@�ӱ����I�H�᪺�ȳ��]��0
		for (int i = this->controlPointIndex[rgbaType][controlPointSize - 1] + 1; i < this->size; i++)
		{
			this->rgba[i][rgbaType] = 0.0;
		}
	}
}