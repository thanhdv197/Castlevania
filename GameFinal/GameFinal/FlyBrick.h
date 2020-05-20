#pragma once
#include "GameObject.h"
#include "Bricks.h"

class CFlyBrick : public CGameObject
{
public:

	CFlyBrick();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

