#pragma once
#include "GameObject.h"
#include "Stair.h"

#define	BOTTOM_STAIR_WIDTH	8
#define BOTTOM_STAIR_HEIGHT	64

#define MAX_STAIR	10

class CBottomStair : public CGameObject
{
	int numStair;
	CStair * stair[MAX_STAIR];
public:
	CBottomStair(float _x, float _y, int _nx, int state_stair, int _numStair);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual bool CollisionPlayer(CGameObject * player) { return false; }
};

