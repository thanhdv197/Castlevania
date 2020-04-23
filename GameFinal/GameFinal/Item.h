#pragma once
#include "GameObject.h"

#define ITEM_STATE_NONE	0
#define ITEM_STATE_DIE	1
#define ITEM_STATE_DEAD	2
#define ITEM_STATE_ENABLE	3

class CItem : public CGameObject
{
public:
	bool isDie;
	bool isEnable;
	bool isItem;

	CItem();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

