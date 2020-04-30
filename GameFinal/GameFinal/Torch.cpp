#include "Torch.h"

CTorch::CTorch(int itemState)
{
	this->isDead = false;
	this->isEnable = true;

	item = new CItem(itemState);
}

void CTorch::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable && !isDead)
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

	item->Update(dt, coObjects);
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
			item->SetPosition(x, y);
			item->Render();
		}
	}
}

void CTorch::GetPosition(float &x, float &y)
{
	x = this->x;
	y = this->y;
}