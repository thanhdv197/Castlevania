#pragma once
#include "GameObject.h"
#include "Torch.h"
#include "Candle.h"
#include "Army.h"

#define WHIP_ANI_RIGHT_LV1	0
#define WHIP_ANI_LEFT_LV1	1

#define WHIP_ANI_RIGHT_LV2	2
#define WHIP_ANI_LEFT_LV2	3

#define WHIP_ANI_RIGHT_LV3	4
#define WHIP_ANI_LEFT_LV3	5

#define WHIP_ANI_RIGHT_LV4	6
#define WHIP_ANI_LEFT_LV4	7

#define WHIP_ANI_RIGHT_LV5	8
#define WHIP_ANI_LEFT_LV5	9

class CWhip : public CGameObject
{
	int ani;
	int level;

public:
	bool isAttack = false;

	CWhip();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void SetPosition(float x, float y);
	void SetDirection(int nx);
	int GetDirection();
	int GetCurrentFrame() { return animation_set->at(ani)->GetCurrentFrame(); }
	void SetCurrentFrame(int frame) { animation_set->at(ani)->SetCurrentFrame(frame); }

	int GetLevel() { return this->level; }
	void LevelUp()
	{
		if (this->level < 5)
		{
			this->level += 1;
		}
	}

};

