#pragma once
#include "GameObject.h"

class CBricks : public CGameObject
{
	int width, height;

public:
	CBricks(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	~CBricks();
};

