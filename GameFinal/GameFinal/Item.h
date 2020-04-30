#pragma once
#include "GameObject.h"
#include "Bricks.h"

#define ITEM_STATE_NONE	7
#define ITEM_STATE_ITEM_WHIP	0
#define ITEM_STATE_ITEM_KNIFE	1
#define ITEM_STATE_ITEM_AXE	2
#define ITEM_STATE_ITEM_BOMERANG	3
#define ITEM_STATE_ITEM_SMALL_HEART	4
#define ITEM_STATE_ITEM_BIG_HEART	5
#define ITEM_STATE_ITEM_FIRE	6

class CItem : public CGameObject
{
	int state;

	int randomItem;

public:
	bool isEnable;

	CItem(int itemType);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	int GetState() { return this->state; }

	void RanDom(int r);

	void SetPosition(int x, int y);
	void GetPosition(int &_x, int &_y);
};

