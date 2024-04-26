#include <iostream>
#include "colorMap2D.h"

void region::setRegionPoints(float centerX, float centerY, float passW, float passH)
{
	//算此region中心點
	this->center.x = centerX;
	this->center.y = centerY;
	this->center.z = 0.0;
	//算此region的四個頂點
	this->points[0].x = centerX - passW / 2.0; this->points[0].y = centerY - passH / 2.0; this->points[0].z = 0.0;
	this->points[1].x = centerX + passW / 2.0; this->points[1].y = centerY - passH / 2.0; this->points[1].z = 0.0;
	this->points[2].x = centerX + passW / 2.0; this->points[2].y = centerY + passH / 2.0; this->points[2].z = 0.0;
	this->points[3].x = centerX - passW / 2.0; this->points[3].y = centerY + passH / 2.0; this->points[3].z = 0.0;
	//指定此region的長、寬
	this->width = passW;
	this->height = passH;
}

void region::setRegionColor(float passR, float passG, float passB, float passA)
{
	this->r = passR;
	this->g = passG;
	this->b = passB;
	this->a = passA;
}

bool region::checkIsInRegion(float passX, float passY)
{
	float compensation = 2.0;
	//以center外擴寬、高的一半來做檢查(並設寬限值)
	if (passX >= (center.x - width / 2.0 - compensation) && passX <= (center.x + width / 2.0 + compensation)
		&& passY >= (center.y - height / 2.0 - compensation) && passY <= (center.y + height / 2.0 + compensation))
	{
		return true;
	}
	else
	{
		return	false;
	}
}

//找點擊調整的點，有找到就回傳true，否則回傳false
bool region::beforeAdjustRegionBoundary(float passX, float passY)
{
	float compensation = 2.0;
	for (int i = 0; i < 4; i++)
	{
		//以每個點外擴寬限值來做檢查
		if (passX >= (this->points[i].x - compensation) && passX <= (this->points[i].x + compensation) &&
			passY >= (this->points[i].y - compensation) && passY <= (this->points[i].y + compensation))
		{
			adjustPtIdx = i;
			return true;
		}
	}
	adjustPtIdx = -1;
	return false;
}

void region::onAdjustRegionBoundary(float passX, float passY)
{
	//動態重新計算四個頂點
	//左下
	if (adjustPtIdx == 0)
	{
		this->setRegionPoints((passX + this->points[2].x) / 2.0, (passY + this->points[2].y) / 2.0, abs(this->points[2].x - passX), abs(this->points[2].y - passY));
	}
	//右下
	else if (adjustPtIdx == 1)
	{
		this->setRegionPoints((passX + this->points[3].x) / 2.0, (passY + this->points[3].y) / 2.0, abs(this->points[3].x - passX), abs(this->points[3].y - passY));
	}
	//右上
	else if (adjustPtIdx == 2)
	{
		this->setRegionPoints((passX + this->points[0].x) / 2.0, (passY + this->points[0].y) / 2.0, abs(this->points[0].x - passX), abs(this->points[0].y - passY));
	}
	//左上
	else if (adjustPtIdx == 3)
	{
		this->setRegionPoints((passX + this->points[1].x) / 2.0, (passY + this->points[1].y) / 2.0, abs(this->points[1].x - passX), abs(this->points[1].y - passY));
	}
}

void region::afterAdjustRegionBoundary(float passX, float passY)
{

}

region::region()
{

}

region::~region()
{

}

//依照給定中心點、長寬、RGB來設定該region資訊
//當點擊menu item時呼叫
//當點擊畫面上的位置時呼叫
void colorMap2D::addNewRegion(float centerX, float centerY, float passW, float passH, float passR, float passG, float passB, float passA)
{
	//新增的region邊界超出範圍
	if (centerX + passW / 2.0 >= maxXSize || centerX - passW / 2.0 < 0 || centerY + passH / 2.0 >= maxYSize || centerY - passH / 2.0 < 0)
	{
		return;
	}
	region tempRegion;
	//tempRegion.calculateContainingVoxel();
	tempRegion.setRegionPoints(centerX, centerY, passW, passH);
	tempRegion.setRegionColor(passR, passG, passB, passA);
	this->Regions.push_back(tempRegion);
}

//當右鍵點擊畫面上region時呼叫
void colorMap2D::deleteRegion(float passX, float passY)
{
	auto it = this->Regions.begin();
	while (it != this->Regions.end())
	{
		//刪除點擊位置有region的該region
		if (it->checkIsInRegion(passX, passY))
		{
			it = this->Regions.erase(it);
		}
		else
		{
			++it;
		}
	}
}

//有找到符合的region就回傳true，否則回傳false
bool colorMap2D::checkAndFindAdjustRegion(float passX, float passY)
{
	for (int i = 0; i < this->Regions.size(); i++)
	{
		if (this->Regions[i].checkIsInRegion(passX, passY))
		{
			onAdjustRegionIdx = i;
			return true;
		}
	}
	onAdjustRegionIdx = -1;
	return false;
}

void colorMap2D::calculateColorMap2DRegions()
{
	//clear the map to 0
	for (int i = 0; i < this->maxXSize; i++)
	{
		for (int j = 0; j < this->maxYSize; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				this->colorMap2DRegions[i * this->maxYSize * 4 + j * 4 + k] = 0;
			}
		}
	}
	//將有region地方的顏色設定成該region的顏色
	for (int i = 0; i < this->Regions.size(); i++)
	{
		for (int j = this->Regions[i].points[0].x; j < this->Regions[i].points[2].x; j++)
		{
			for (int k = this->Regions[i].points[0].y; k < this->Regions[i].points[2].y; k++)
			{
				this->colorMap2DRegions[j * this->maxYSize * 4 + k * 4 + 0] = (unsigned char)this->Regions[i].r;
				this->colorMap2DRegions[j * this->maxYSize * 4 + k * 4 + 1] = (unsigned char)this->Regions[i].g;
				this->colorMap2DRegions[j * this->maxYSize * 4 + k * 4 + 2] = (unsigned char)this->Regions[i].b;
				this->colorMap2DRegions[j * this->maxYSize * 4 + k * 4 + 3] = (unsigned char)this->Regions[i].a;
			}
		}
	}
}

void colorMap2D::setMaxXYSize(int passX, int passY)
{
	this->maxXSize = passX;
	this->maxYSize = passY;
	//allocate memory for 2D texture(2D color map)
	this->colorMap2DRegions = new unsigned char[this->maxXSize * this->maxYSize * 4];
	//clear the map to 0
	for (int i = 0; i < this->maxXSize; i++)
	{
		for (int j = 0; j < this->maxYSize; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				this->colorMap2DRegions[i * this->maxYSize * 4 + j * 4 + k] = 0;
			}
		}
	}
}

colorMap2D::colorMap2D()
{
	this->Regions.reserve(10);
}

colorMap2D::~colorMap2D()
{

}