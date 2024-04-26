#pragma once
#include <vector>
#include <algorithm>
#include "voxel.h"

/// <summary>
/// �C�@��function value�����쪺r, g, b, a��(�i�ѨϥΪ̩w�q�����I)�A���ݩ��function value��voxels
/// </summary>
class colorMap
{
public:
	colorMap(float maxFV);
	~colorMap();
	//�Hfunction value�Ȭ�index�A�s��function value�W��r,g,b,a��
	float** rgba;
	//�sr,g,b,a�|��spline�W�A�ϥΪ̩w�q���I��index
	std::vector<int> controlPointIndex[4];
	//�Hfunction value�Ȭ�index�A�s��function value����
	float* functionValue;
	//�`�@���X��function value(ex: 256�� 0~255)
	int size;
	void userDefineControlPoint(int idx, float colorValue, int rgbaType);
	bool userCancelControlPoint(int idx, int rgbaType);
private:
	void recalculateColorSpline(int rgbaType);
};