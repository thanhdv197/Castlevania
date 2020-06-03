#pragma once
#include "GameObject.h"
#include "bricks.h"

#define STATE_NONE	0
#define STATE_THROW	1

#define ANI_THROW	9

class CBone : public CGameObject
{
	DWORD timeThrow;

	int ny;

public:

	bool isEnable;

	CBone();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);

	void SetPosition(float x, float y);
	void SetNx(int _nx) { this->nx = _nx; }
};

