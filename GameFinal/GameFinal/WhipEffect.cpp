#include "WhipEffect.h"

CWhipEffect::CWhipEffect()
{
	isEnable = true;

	this->timeDisplay = 0;

	SetAnimationSet(CAnimationSets::GetInstance()->Get(46));
}

void CWhipEffect::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	timeDisplay += dt;
	if (timeDisplay > 50)
		isEnable = false;
}

void CWhipEffect::Render()
{
	if (isEnable)
		animation_set->at(0)->Render(x, y);
}

void CWhipEffect::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}