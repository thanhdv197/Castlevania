#pragma once
#include "GameObject.h"
#include "bricks.h"

#define STATE_NONE	0
#define STATE_THROW	1

#define ANI_THROW	9

#define	TIME_UP	500
#define TIME_DOWN	1200

#define BONE_WIDTH	16
#define BONE_HEIGHT	16

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

