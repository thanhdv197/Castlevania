#pragma once
#include "GameObject.h"
#include "Item.h"
#include "Whip.h"
#include "Weapon.h"

#define SIMON_WALKING_SPEED		0.1f 
//0.1f
#define SIMON_JUMP_SPEED_Y		0.3f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.001f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_SIT				500
#define SIMON_STATE_ATTACK			600
#define SIMON_STATE_HURT			700

#define SIMON_ANI_IDLE_RIGHT		0
#define SIMON_ANI_IDLE_LEFT			1

#define SIMON_ANI_WALKING_RIGHT		2
#define SIMON_ANI_WALKING_LEFT		3

#define SIMON_ANI_JUMP_RIGHT		4
#define SIMON_ANI_JUMP_LEFT			5

#define SIMON_ANI_SIT_RIGHT			6
#define SIMON_ANI_SIT_LEFT			7

#define SIMON_ANI_ATTACK_RIGHT		8
#define SIMON_ANI_ATTACK_LEFT		9

#define SIMON_ANI_DIE				10

#define SIMON_ANI_SIT_ATTACK_RIGHT	11
#define SIMON_ANI_SIT_ATTACK_LEFT	12

#define SIMON_BBOX_WIDTH  16
#define SIMON_BBOX_HEIGHT 30
#define SIMON_BBOX_HEIGHT_SIT 23

#define SIMON_UNTOUCHABLE_TIME 5000


class CSimon : public CGameObject
{
	//static CSimon* __instance;

	int untouchable;
	DWORD untouchable_start;

	float start_x;
	float start_y;

	int ani;

	CWhip * whip;

	int stateWeapon;

	CWeapon * weapon;

	int blood;
	int alive;
	int heart;
	int score;

public:
	//static CSimon* GetInstance();

	// check active
	bool isAttack;
	bool isJump;
	bool isSit;

	// check type attack
	bool usingWhip;
	bool isFlyingWeapon;

	//CSimon();
	CSimon(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	int GetDirection() { return this->nx; }

	void SetPosition(float x, float y);
};