#pragma once
#include "GameObject.h"
#include "Item.h"

#define WEAPON_FLY_SPEED 0.05f;

#define WEAPON_STATE_NONE	100
#define WEAPON_STATE_KNIFE	200

#define WEAPON_ANI_KNIFE_RIGHT	0
#define WEAPON_ANI_KNIFE_LEFT	1

class CWeapon : public CGameObject
{
	int ani;
	int state;

public:

	bool isEnable;

	CWeapon(float x, float y, int nx);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void SetPosition(float x, float y);
	void SetDirection(int nx);
	int GetDirection();
	int GetCurrentFrame() { return animation_set->at(ani)->GetCurrentFrame(); }
	void SetCurrentFrame(int frame) { animation_set->at(ani)->SetCurrentFrame(frame); }

	int GetState() { return this->state; }

};



