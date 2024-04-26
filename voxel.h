#pragma once
#include "vec.h"
class voxel
{
public:
	voxel();
	voxel(vec pos, float fv, vec grad);
	~voxel();
	vec position;
	float functionValue = 0.0;
	vec gradient;
	int colorMapIdx = 0;
private:

};