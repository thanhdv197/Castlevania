#include "HitEffect.h"

void CHitEffect::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	//CGameObject::Update(dt, coObjects);

	if (isEnable)
	{
		timeDisplay += dt;

		if (timeDisplay > 100)
			isEnable = false;
	}
}

void CHitEffect::Render()
{
	if (isEnable)
	{
		animation_set->at(0)->Render(x, y);
	}
}
