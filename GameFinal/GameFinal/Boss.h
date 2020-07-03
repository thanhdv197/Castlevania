#pragma once
#include "GameObject.h"
#include "WhipEffect.h"
#include "Bricks.h"
#include "Game.h"

#define STATE_BOSS_STAND	100
#define STATE_BOSS_FLY_1	200
#define STATE_BOSS_FLY_2	700
#define STATE_BOSS_WAIT		300
#define STATE_BOSS_DIE		400
#define STATE_BOSS_ATTACK	500
#define STATE_GLOBULAR		600

#define ANI_BOSS_STAND	0
#define ANI_BOSS_FLY	1
#define ANI_BOSS_DIE	2
#define ANI_GLOBULAR	3

#define LIMIT_X	768
#define DISTANCE_CHANGE_STATE_FLY	100
#define DISTANCE_Y_CHANGE_WAIT	30
#define DISTANCE_Y_CHANGE_FLY	5
#define DISTANCE_Y_FINISH_FLY_2	40
#define DISTANCE_X_FINISH_FLY_2	80

#define TIME_WAIT	1200

#define BOSS_WIDTH	48
#define BOSS_HEIGHT	24
#define	ITEM_WIDTH	16
#define	ITEM_HEIGHT	16

#define START_X	580
#define START_Y	60

class CBoss : public CGameObject
{
	int ani;

	int blood;

	float start_x, start_y;

	int ny;

	DWORD timeChangeState;
	DWORD timeDead;

	CWhipEffect * whipEffect;

	int oldState;
public:

	bool isEnable;
	bool isAttacked;
	bool isDisplay;

	CBoss();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);

	void LostBlood(int _blood) { this->blood -= _blood; this->bloodBoss = this->blood; }
	int GetBlood() { return this->blood; }

	void Reset() { x = start_x; y = start_y; }

	virtual bool CollisionPlayer(CGameObject * player) { return false; }
};

