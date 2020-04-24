#include "Whip.h"

CWhip::CWhip(float x, float y, int nx) : CGameObject()
{
	x = x;
	y = y;
	nx = nx;

	SetAnimationSet(CAnimationSets::GetInstance()->Get(49));
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CItem*>(coObjects->at(i))) {
			CItem* item = dynamic_cast<CItem*>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			item->GetBoundingBox(l2, t2, r2, b2);

			if (t1 <= b2 && b1 >= t2 && l1 <= r2 && r1 >= l2)
			{
				item->SetState(ITEM_STATE_DESTROY);
				//item->isEnable = true;
			}
		}
		
	}
}

void CWhip::Render()
{
	if (nx > 0)
	{
		ani = 0;
	}
	else
		ani = 1;

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CWhip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (animation_set->at(ani)->GetCurrentFrame() == 2)
	{
		left = x;
		top = y;
		right = left + 54;
		bottom = top + 16;
	}
	
}

void CWhip::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CWhip::SetDirection(int nx)
{
	this->nx = nx;
}

int CWhip::GetDirection()
{
	return this->nx;
}