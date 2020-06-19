#pragma once
#include "GameObject.h"
#include "WhipEffect.h"
#include "DieEffect.h"
#include "Bricks.h"
#include "Game.h"

#define STATE_BAT_STAND	100
#define STATE_BAT_FLY	300
#define STATE_BAT_DOWN	400
#define STATE_ITEM	200

#define ANI_ITEM_WHIP	0
#define ANI_ITEM_KNIFE	1
#define ANI_ITEM_AXE	2
#define ANI_ITEM_BOMERANG	3
#define ANI_ITEM_SMALL_HEART	4
#define ANI_ITEM_BIG_HEART	5
#define ANI_ITEM_FIRE	6

#define ANI_BAT_STAND	7
#define ANI_BAT_FLY_LEFT	8
#define ANI_BAT_FLY_RIGHT	9

#define DISTANCE_MIN	30
#define DISTANCE_MAX	80

#define BAT_WIDTH	16
#define BAT_HEIGHT	16
#define	ITEM_WIDTH	16
#define	ITEM_HEIGHT	16

class CBat : public CGameObject
{
	int ani;

	int item;

	int blood;

	CWhipEffect * whipEffect;

	CDieEffect * dieEffect;

	DWORD timeDown;

public:

	bool isEnable;
	bool isDisplay;
	bool isAttacked;

	CBat(int _item);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);

	int GetItem() { return this->item; }

	void LostBlood(int _blood) { this->blood -= _blood; }
	int GetBlood() { return this->blood; }

	virtual bool CollisionPlayer(CGameObject * player) { return false; }
};

