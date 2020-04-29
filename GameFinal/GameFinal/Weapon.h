#pragma once
#include "GameObject.h"
#include "Item.h"

#define WEAPON_FLY_SPEED 0.05f;

#define WEAPON_STATE_NONE	100
#define WEAPON_STATE_KNIFE	200
#define WEAPON_STATE_AXE	300
#define WEAPON_STATE_BOMERANG	400
#define WEAPON_STATE_FIRE	500

#define WEAPON_ANI_KNIFE_RIGHT	0
#define WEAPON_ANI_KNIFE_LEFT	1

#define WEAPON_ANI_AXE_RIGHT	2
#define WEAPON_ANI_AXE_LEFT	3

#define WEAPON_ANI_BOMERANG	4

#define WEAPON_ANI_FIRE	5


class CWeapon : public CGameObject
{
	int ani;
	int state;

public:

	bool isEnable;

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

};



