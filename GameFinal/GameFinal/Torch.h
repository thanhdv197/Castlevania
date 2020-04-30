#pragma once
#include "GameObject.h"
#include "Item.h"

class CTorch : public CGameObject
{
	CItem * item;

public:
	bool isDead;
	bool isEnable;

	CTorch(int itemState);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	void GetPosition(float &x, float &y);
};