#pragma once
#include "GameObject.h"
#include "Game.h"

#define	STATE_1	1
#define STATE_2	2
#define STATE_3	3

#define ANI_RIGHT_1	1
#define ANI_RIGHT_2	0
#define ANI_RIGHT_3	3
#define ANI_LEFT_1	5
#define ANI_LEFT_2	4
#define ANI_LEFT_3	7

class CStair : public CGameObject
{
	int ani;
public:
	CStair(int _nx, int _state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);

	void SetNx(int _nx) { this->nx = _nx; }

	virtual bool CollisionPlayer(CGameObject * player) { return false; }
};

