#include "vec.h"

//�w�]�غc�禡
vec::vec()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

//�غc�禡
vec::vec(float passX, float passY, float passZ)
{
	this->x = passX;
	this->y = passY;
	this->z = passZ;
}

vec::~vec()
{
}