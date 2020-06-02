#include "DieEffect.h"

CDieEffect::CDieEffect()
{
	isEnable = true;

	this->timeDisplay = 0;

	SetAnimationSet(CAnimationSets::GetInstance()->Get(46));
}

void CDieEffect::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	timeDisplay += dt;
	if (timeDisplay > 200)
		isEnable = false;
}

void CDieEffect::Render()
{
	if (isEnable)
		animation_set->at(1)->Render(x, y);
}

void CDieEffect::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}
