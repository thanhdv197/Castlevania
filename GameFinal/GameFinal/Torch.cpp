#include "Torch.h"

CTorch::CTorch()
{
	this->isEnable = true;
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
}

void CTorch::Render()
{
	if (isEnable)
	{
		animation_set->at(0)->Render(x, y);
		RenderBoundingBox();

	}
}

void CTorch::GetPosition(float &x, float &y)
{
	x = this->x;
	y = this->y;
}