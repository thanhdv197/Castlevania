#pragma once
#include "GameObject.h"

class CDieEffect : public CGameObject
{
	DWORD timeDisplay;
public:
	bool isEnable;

	CDieEffect();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

