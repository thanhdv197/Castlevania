#pragma once
#include "GameObject.h"
#include "WhipEffect.h"
#include "DieEffect.h"
#include "Bricks.h"
#include "Game.h"

#define STATE_TOAD_STAND	100
#define STATE_TOAD_JUMP	300
#define STATE_ITEM	200

#define ANI_ITEM_WHIP	0
#define ANI_ITEM_KNIFE	1
#define ANI_ITEM_AXE	2
#define ANI_ITEM_BOMERANG	3
#define ANI_ITEM_SMALL_HEART	4
#define ANI_ITEM_BIG_HEART	5
#define ANI_ITEM_FIRE	6

#define ANI_TOAD_STAND_LEFT	7
#define ANI_TOAD_JUMP_LEFT	8
#define ANI_TOAD_STAND_RIGHT	9
#define ANI_TOAD_JUMP_RIGHT	10

#define DISTANC_CHANGE_STATE_JUMP	100

#define TOAD_GRAVITY	0.001f
#define TOAD_JUMP_SPEED	0.25f
#define TOAD_SPEED	0.1f
#define ITEM_DROP	0.05f

#define TOAD_WIDTH	16
#define TOAD_HEIGHT	16
#define	ITEM_WIDTH	16
#define	ITEM_HEIGHT	16

class CToad : public CGameObject
{
	int ani;

	int item;

	int blood;

	CWhipEffect * whipEffect;

	CDieEffect * dieEffect;
public:

	bool isEnable;
	bool isDisplay;
	bool isAttacked;

	CToad(int _item);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);

	int GetItem() { return this->item; }

	void LostBlood(int _blood) { this->blood -= _blood; }
	int GetBlood() { return this->blood; }

	virtual bool CollisionPlayer(CGameObject * player) { return false; }
};

