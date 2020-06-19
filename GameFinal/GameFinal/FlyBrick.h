#pragma once
#include "GameObject.h"
#include "Bricks.h"

#define BRICK_WIDTH	32
#define	BRICK_HEIGHT	16

#define	SPEED_VX	0.02f

class CFlyBrick : public CGameObject
{
public:

	CFlyBrick();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual bool CollisionPlayer(CGameObject * player) { return false; }
};

