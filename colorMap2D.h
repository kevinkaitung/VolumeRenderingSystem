#pragma once
#include <vector>
#include "vec.h"
#include "voxel.h"

class region
{
public:
	region();
	~region();
	//儲存此region中心點
	vec center;
	//儲存構成此region的四個頂點(only 2D)
	vec points[4];
	//儲存此region的高、寬
	float height = 0.0, width = 0.0;
	//此region的顯示color
	float r = 0.0, g = 0.0, b = 0.0, a = 0.0;
	//儲存此region調整中的點
	int adjustPtIdx = -1;
	//儲存屬於此region的voxels
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
