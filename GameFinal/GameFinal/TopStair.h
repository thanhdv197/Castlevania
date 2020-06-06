#pragma once
#include "GameObject.h"

#define	TOP_STAIR_WIDTH	8
#define TOP_STAIR_HEIGHT	64

class CTopStair : public CGameObject
{
public:
	CTopStair(float _x, float _y, int _nx);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

