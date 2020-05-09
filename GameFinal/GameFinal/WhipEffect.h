#pragma once
#include "GameObject.h"

class CWhipEffect : public CGameObject
{
public:
	bool isEnable;

	CWhipEffect();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

