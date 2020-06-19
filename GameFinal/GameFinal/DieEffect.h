#pragma once
#include "GameObject.h"

#define TIME_END	200

class CDieEffect : public CGameObject
{
	DWORD timeDisplay;
public:
	bool isEnable;

	CDieEffect();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual bool CollisionPlayer(CGameObject * player) { return false; }
};

