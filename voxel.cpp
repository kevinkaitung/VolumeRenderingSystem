#include "voxel.h"

voxel::voxel()
{

}

voxel::voxel(vec pos, float fv, vec grad)
{
	this->position = pos;
	this->functionValue = fv;
	this->gradient = grad;
}

voxel::~voxel()
{
}