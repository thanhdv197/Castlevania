#include "WhipEffect.h"

CWhipEffect::CWhipEffect()
{
	isEnable = true;

	SetAnimationSet(CAnimationSets::GetInstance()->Get(46));
}

void CWhipEffect::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
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