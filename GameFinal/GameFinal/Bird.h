#pragma once
#include "GameObject.h"
#include "WhipEffect.h"
#include "DieEffect.h"
#include "Bricks.h"

#define STATE_BIRD_STAND	100
#define STATE_BIRD_FLY	300
#define STATE_BIRD_FLY_WAIT	400
#define STATE_ITEM	200

#define ANI_ITEM_WHIP	0
#define ANI_ITEM_KNIFE	1
#define ANI_ITEM_AXE	2
#define ANI_ITEM_BOMERANG	3
#define ANI_ITEM_SMALL_HEART	4
#define ANI_ITEM_BIG_HEART	5
#define ANI_ITEM_FIRE	6

#define ANI_BIRD_STAND_LEFT	7
#define ANI_BIRD_FLY_LEFT	8
#define ANI_BIRD_STAND_RIGHT	9
#define ANI_BIRD_FLY_RIGHT	10

class CBird : public CGameObject
{
	int ani;

	int item;

	int blood;

	int ny;

	DWORD timeChangeState;

	CWhipEffect * whipEffect;

	CDieEffect * dieEffect;
public:

	bool isEnable;
	bool isAttacked;

	CBird(int item);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);

	void RanDom(int r);

	int GetItem() { return this->item; }

	void LostBlood(int _blood) { this->blood -= _blood; }
	int GetBlood() { return this->blood; }
};

