#pragma once
#include "GameObject.h"
#include "WhipEffect.h"
#include "Bricks.h"

#define STATE_BOSS_STAND	100
#define STATE_BOSS_FLY	200
#define STATE_BOSS_FLY_WAIT	300
#define STATE_BOSS_DIE	400
#define STATE_BOSS_ATTACK	500

#define ANI_BOSS_STAND	0
#define ANI_BOSS_FLY	1
#define ANI_BOSS_DIE	2
#define ANI_GLOBULAR	3

class CBoss : public CGameObject
{
	int ani;

	int item;

	int blood;

	int ny;

	DWORD timeChangeState;
	DWORD timeDead;

	CWhipEffect * whipEffect;
public:

	bool isEnable;
	bool isAttacked;

	CBoss();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);

	void LostBlood(int _blood) { this->blood -= _blood; }
	int GetBlood() { return this->blood; }
};

