#include "Torch.h"

CTorch::CTorch()
{
	this->isEnable = true;

	this->isDead = false;
}

void CTorch::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		left = x;
		top = y;
		right = x + 16;
		bottom = y + 32;
	}
	
}

void CTorch::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (isDead)
	{
		timeDisplay += dt;

		if (timeDisplay > 300)
			this->isEnable = false;
	}
		
}

void CTorch::Render()
{
	if (isEnable)
	{
		if (!isDead)
		{
			animation_set->at(0)->Render(x, y);
			RenderBoundingBox();
		}
		else
		{
			animation_set->at(1)->Render(x+2, y+8);
		}
	}
}

void CTorch::GetPosition(float &x, float &y)
{
	x = this->x;
	y = this->y;
}