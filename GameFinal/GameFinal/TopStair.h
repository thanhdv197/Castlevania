#pragma once
#include "GameObject.h"

class CTopStair : public CGameObject
{
public:
	CTopStair(float _x, float _y, int _nx);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

