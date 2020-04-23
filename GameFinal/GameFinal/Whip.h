#pragma once
#include "GameObject.h"
#include "Item.h"

class CWhip : public CGameObject
{
	int ani;

public:
	CWhip(float x, float y, int nx);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void SetPosition(float x, float y);
	void SetDirection(int nx);
	int GetDirection();

};

