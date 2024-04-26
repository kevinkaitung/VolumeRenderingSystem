#pragma once
#include <vector>
#include <algorithm>
#include "voxel.h"

/// <summary>
/// 每一個function value對應到的r, g, b, a值(可由使用者定義取樣點)，及屬於該function value的voxels
/// </summary>
class colorMap
{
public:
	colorMap(float maxFV);
	~colorMap();
	//以function value值為index，存該function value上的r,g,b,a值
	float** rgba;
	//存r,g,b,a四條spline上，使用者定義的點的index
	std::vector<int> controlPointIndex[4];
	//以function value值為index，存該function value的值
	float* functionValue;
	//總共有幾種function value(ex: 256種 0~255)
	int size;
	void userDefineControlPoint(int idx, float colorValue, int rgbaType);
	bool userCancelControlPoint(int idx, int rgbaType);
private:
	void recalculateColorSpline(int rgbaType);
};