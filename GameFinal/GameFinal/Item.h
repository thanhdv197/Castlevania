#pragma once
#include "GameObject.h"

#define ITEM_STATE_NONE	0
#define ITEM_STATE_DESTROY	1
#define ITEM_STATE_ITEM_WHIP	2
#define ITEM_STATE_ITEM_KNIFE	3

class CItem : public CGameObject
{
	int state;

	int randomItem;

public:
	bool isDead;
	bool isEnable;

	CItem();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void SetState(int state);
	int GetState() { return this->state; }
};

