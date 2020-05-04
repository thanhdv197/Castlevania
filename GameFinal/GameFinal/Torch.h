#pragma once
#include "GameObject.h"

class CTorch : public CGameObject
{
	DWORD timeDisplay = 0;
public:
	bool isEnable;
	bool isDead;

	CTorch();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	void GetPosition(float &x, float &y);
};