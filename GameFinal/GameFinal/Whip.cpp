#include "Whip.h"

CWhip::CWhip(float x, float y, int nx) : CGameObject()
{
	x = x;
	y = y;
	nx = nx;

	this->level = 1;

	SetAnimationSet(CAnimationSets::GetInstance()->Get(49));
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CTorch*>(coObjects->at(i))) {
			CTorch* torch = dynamic_cast<CTorch*>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			torch->GetBoundingBox(l2, t2, r2, b2);

			if (t1 <= b2 && b1 >= t2 && l1 <= r2 && r1 >= l2)
			{
				//torch->isEnable = false;
				torch->isDead = true;

			}
		}
		else if (dynamic_cast<CItem *>(coObjects->at(i)))
		{
			CItem *item = dynamic_cast<CItem *>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			item->GetBoundingBox(l2, t2, r2, b2);

			if (t1 <= b2 && b1 >= t2 && l1 <= r2 && r1 >= l2)
			{
				//torch->isEnable = false;
				item->isEnable = false;
			}

		}
		
	}
}

void CWhip::Render()
{
	if (this->level == 1)
	{
		if (nx > 0)
		{
			ani = WHIP_ANI_RIGHT_LV1;
		}
		else
			ani = WHIP_ANI_LEFT_LV1;
	}
	else if (this->level == 2)
	{
		if (nx > 0)
		{
			ani = WHIP_ANI_RIGHT_LV2;
		}
		else
			ani = WHIP_ANI_LEFT_LV2;
	}
	else if (this->level == 3)
	{
		if (nx > 0)
		{
			ani = WHIP_ANI_RIGHT_LV3;
		}
		else
			ani = WHIP_ANI_LEFT_LV3;
	}
	else if (this->level == 4)
	{
		if (nx > 0)
		{
			ani = WHIP_ANI_RIGHT_LV4;
		}
		else
			ani = WHIP_ANI_LEFT_LV4;
	}
	else
	{
		if (nx > 0)
		{
			ani = WHIP_ANI_RIGHT_LV5;
		}
		else
			ani = WHIP_ANI_LEFT_LV5;
	}
	
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CWhip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (animation_set->at(ani)->GetCurrentFrame() == 2)
	{
		if (this->level == 1)
		{
			left = x;
			top = y;
			right = left + 54;
			bottom = top + 16;
		}
		else
		{
			left = x;
			top = y;
			right = left + 73;
			bottom = top + 16;
		}
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