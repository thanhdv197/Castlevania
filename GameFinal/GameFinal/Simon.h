#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Weapon.h"
#include "Candle.h"
#include "Army.h"
#include "Zombie.h"
#include "Bat.h"
#include "Flea.h"
#include "Bird.h"
#include "Toad.h"
#include "Skeleton.h"

#define SIMON_WALKING_SPEED		0.1f 
//0.1f
#define SIMON_JUMP_SPEED_Y		0.4f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
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

#define SIMON_ANI_LEVELUP_RIGHT	13
#define SIMON_ANI_LEVELUP_LEFT	14

#define SIMON_BBOX_WIDTH  16
#define SIMON_BBOX_HEIGHT 30
#define SIMON_BBOX_HEIGHT_SIT 23

#define SIMON_UNTOUCHABLE_TIME 5000


class CSimon : public CGameObject
{
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

	DWORD timeLevelUp;

public:
	// check active
	bool isAttack;
	bool isJump;
	bool isSit;
	bool isLevelUp;

	// check type attack
	bool usingWhip;
	bool isFlyingWeapon;

	CSimon(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	int GetDirection() { return this->nx; }

	void CollisionItem(int item);

	int GetBlood() { return this->blood; }
	void SetBlood(int _blood) { this->blood = _blood; }

	int GetAlive() { return this->alive; }
	void SetAlive(int _alive) { this->alive = _alive; }

	int GetHeart() { return this->heart; }
	void SetHeart(int _heart) { this->heart = _heart; }

	int GetScores() { return this->score; }
	void SetScores(int _scores) { this->score = _scores; }
	void AddScores(int _scores) { this->score += _scores; }

	int GetWeapon() { return this->stateWeapon; }
	void SetWeapon(int _weapon) { this->stateWeapon = _weapon; }
	
	void SetWhip(int _level) { whip->SetLevel(_level); }
	int GetLevelWhip() { return whip->GetLevel(); }

	void LostBlood(int _blood);
};