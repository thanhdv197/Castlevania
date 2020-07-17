#pragma once
#include "GameObject.h"
#include "WhipEffect.h"
#include "DieEffect.h"
#include "Bricks.h"
#include "Bone.h"
#include "Game.h"

#define STATE_SKELETON_STAND	100
#define STATE_SKELETON_JUMP	300
#define STATE_ITEM	200

#define ANI_ITEM_WHIP	0
#define ANI_ITEM_KNIFE	1
#define ANI_ITEM_AXE	2
#define ANI_ITEM_BOMERANG	3
#define ANI_ITEM_SMALL_HEART	4
#define ANI_ITEM_BIG_HEART	5
#define ANI_ITEM_FIRE	6

#define ANI_SKELETON_LEFT	7
#define ANI_SKELETON_RIGHT	8
#define ANI_THROW	9

#define SKELETON_WIDTH	16
#define SKELETON_HEIGHT	32
#define	ITEM_WIDTH	16
#define	ITEM_HEIGHT	16

#define DISTANCE_STATE_JUMP	200
#define DISTANCE_Y_JUMP_DOWN	50
#define DISTANCE_STATE_THROW	70
#define TIME_WALK_THROW	300
#define TIME_WALK_THROW_OPPOSITE	600

class CSkeleton : public CGameObject
{
	int ani;

	int item;

	int blood;

	CWhipEffect * whipEffect;

	CDieEffect * dieEffect;

	CBone* bone;

	int start_y, start_x;

	DWORD timeWalkThrow = 0;
public:
	bool isJump;
	bool isOpposite;
	bool isEnable;
	bool isDisplay;
	bool isAttacked;

	CSkeleton(int _item);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);

	int GetItem() { return this->item; }

	void LostBlood(int _blood) { this->blood -= _blood; }
	int GetBlood() { return this->blood; }

	virtual bool CollisionPlayer(CGameObject * player) { return bone->CollisionPlayer(player); }
};

