#pragma once
#include "GameObject.h"

class CBottomStair : public CGameObject
{
public:
	CBottomStair(float _x, float _y, int _nx);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

