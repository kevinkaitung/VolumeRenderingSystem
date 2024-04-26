#pragma once
#include <vector>
#include "vec.h"
#include "voxel.h"

class region
{
public:
	region();
	~region();
	//�x�s��region�����I
	vec center;
	//�x�s�c����region���|�ӳ��I(only 2D)
	vec points[4];
	//�x�s��region�����B�e
	float height = 0.0, width = 0.0;
	//��region�����color
	float r = 0.0, g = 0.0, b = 0.0, a = 0.0;
	//�x�s��region�վ㤤���I
	int adjustPtIdx = -1;
	//�x�s�ݩ�region��voxels
	//std::vector<voxel> regionVoxels;
	int colorMapIdx = 0;
	void setRegionPoints(float centerX, float centerY, float passW, float passH);
	void setRegionColor(float passR, float passG, float passB, float passA);
	bool checkIsInRegion(float passX, float passY);
	bool beforeAdjustRegionBoundary(float passX, float passY);
	void onAdjustRegionBoundary(float passX, float passY);
	void afterAdjustRegionBoundary(float passX, float passY);
};

class colorMap2D
{
public:
	colorMap2D();
	~colorMap2D();
	std::vector<region> Regions;
	int maxXSize = 0, maxYSize = 0;
	int onAdjustRegionIdx = -1;
	unsigned char* colorMap2DRegions;
	void addNewRegion(float centerX, float centerY, float passW, float passH, float passR, float passG, float passB, float passA);
	void deleteRegion(float passX, float passY);
	bool checkAndFindAdjustRegion(float passX, float passY);
	void calculateColorMap2DRegions();
	void setMaxXYSize(int passX, int passY);
private:

};
