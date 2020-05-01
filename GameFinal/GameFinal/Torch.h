#pragma once
#include "GameObject.h"

class CTorch : public CGameObject
{
public:
	bool isEnable;

	CTorch();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	void GetPosition(float &x, float &y);
};