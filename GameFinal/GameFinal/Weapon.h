#pragma once
#include "GameObject.h"
#include "Candle.h"
#include "Torch.h"
#include "Army.h"
#include "Game.h"
#include "Zombie.h"
#include "Bat.h"
#include "Flea.h"
#include "Bird.h"
#include "Toad.h"
#include "Skeleton.h"
#include "Boss.h"
#include "Bricks.h"

#define WEAPON_FLY_SPEED 0.1f

#define WEAPON_STATE_NONE		0
#define WEAPON_STATE_KNIFE		200
#define WEAPON_STATE_AXE		300
#define WEAPON_STATE_BOMERANG	400
#define WEAPON_STATE_FIRE		500

#define WEAPON_ANI_KNIFE_RIGHT	0
#define WEAPON_ANI_KNIFE_LEFT	1

#define WEAPON_ANI_AXE			2

#define WEAPON_ANI_BOMERANG		3

#define WEAPON_ANI_FIRE			4
#define WEAPON_ANI_FIRE_BURN	5

#define TIME_ENABLE_BOMERANG_FIRE		2000
#define TIME_ENABLE_KNIFE_AXE			1000
#define TIME_CHANGE_DIRECTION_BOMERANG	1000
#define TIME_DOWN_FIRE					300
#define TIME_DOWN_AXE					500

class CWeapon : public CGameObject
{
	int ani;
	int state;
	int dame = 0;

	DWORD timeAttack;

public:
	bool isAttack = false;
	bool isBurning;
	bool isEnable;

	CWeapon(int state);

	CWeapon(float x, float y, int nx, int state);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void SetPosition(float x, float y);
	void GetPosition(float &x, float &y);
	void SetDirection(int nx);
	int GetDirection();
	int GetCurrentFrame() { return animation_set->at(ani)->GetCurrentFrame(); }
	void SetCurrentFrame(int frame) { animation_set->at(ani)->SetCurrentFrame(frame); }

	int GetState() { return this->state; }
	virtual void SetState(int state);

	int GetDame() { return this->dame; }

	virtual bool CollisionPlayer(CGameObject * player) { return false; }
};



