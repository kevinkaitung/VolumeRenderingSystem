#include <iostream>
#include "colorMap2D.h"

void region::setRegionPoints(float centerX, float centerY, float passW, float passH)
{
	//�⦹region�����I
	this->center.x = centerX;
	this->center.y = centerY;
	this->center.z = 0.0;
	//�⦹region���|�ӳ��I
	this->points[0].x = centerX - passW / 2.0; this->points[0].y = centerY - passH / 2.0; this->points[0].z = 0.0;
	this->points[1].x = centerX + passW / 2.0; this->points[1].y = centerY - passH / 2.0; this->points[1].z = 0.0;
	this->points[2].x = centerX + passW / 2.0; this->points[2].y = centerY + passH / 2.0; this->points[2].z = 0.0;
	this->points[3].x = centerX - passW / 2.0; this->points[3].y = centerY + passH / 2.0; this->points[3].z = 0.0;
	//���w��region�����B�e
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
	//�Hcenter�~�X�e�B�����@�b�Ӱ��ˬd(�ó]�e����)
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

//���I���վ㪺�I�A�����N�^��true�A�_�h�^��false
bool region::beforeAdjustRegionBoundary(float passX, float passY)
{
	float compensation = 2.0;
	for (int i = 0; i < 4; i++)
	{
		//�H�C���I�~�X�e���ȨӰ��ˬd
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
	//�ʺA���s�p��|�ӳ��I
	//���U
	if (adjustPtIdx == 0)
	{
		this->setRegionPoints((passX + this->points[2].x) / 2.0, (passY + this->points[2].y) / 2.0, abs(this->points[2].x - passX), abs(this->points[2].y - passY));
	}
	//�k�U
	else if (adjustPtIdx == 1)
	{
		this->setRegionPoints((passX + this->points[3].x) / 2.0, (passY + this->points[3].y) / 2.0, abs(this->points[3].x - passX), abs(this->points[3].y - passY));
	}
	//�k�W
	else if (adjustPtIdx == 2)
	{
		this->setRegionPoints((passX + this->points[0].x) / 2.0, (passY + this->points[0].y) / 2.0, abs(this->points[0].x - passX), abs(this->points[0].y - passY));
	}
	//���W
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

//�̷ӵ��w�����I�B���e�BRGB�ӳ]�w��region��T
//���I��menu item�ɩI�s
//���I���e���W����m�ɩI�s
void colorMap2D::addNewRegion(float centerX, float centerY, float passW, float passH, float passR, float passG, float passB, float passA)
{
	//�s�W��region��ɶW�X�d��
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

//��k���I���e���Wregion�ɩI�s
void colorMap2D::deleteRegion(float passX, float passY)
{
	auto it = this->Regions.begin();
	while (it != this->Regions.end())
	{
		//�R���I����m��region����region
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

//�����ŦX��region�N�^��true�A�_�h�^��false
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
	//�N��region�a�誺�C��]�w����region���C��
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