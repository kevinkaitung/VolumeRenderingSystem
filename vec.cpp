#include "vec.h"

//預設建構函式
vec::vec()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

//建構函式
vec::vec(float passX, float passY, float passZ)
{
	this->x = passX;
	this->y = passY;
	this->z = passZ;
}

vec::~vec()
{
}