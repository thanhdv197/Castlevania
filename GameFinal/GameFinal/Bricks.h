#pragma once
#include "GameObject.h"

class CBricks : public CGameObject
{
public:
	CBricks(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual bool CollisionPlayer(CGameObject * player) { return false; }
	~CBricks();
};

