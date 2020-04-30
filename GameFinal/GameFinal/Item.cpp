#include "Item.h"

CItem::CItem(int itemType)
{
	isEnable = false;

	SetAnimationSet(CAnimationSets::GetInstance()->Get(47));

	RanDom(itemType);
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	//CGameObject::Update(dt);
	x += dx;
	y += 0.5f*dt;

}

void CItem::Render()
{
	if (!isEnable)
	{
		if (this->state != ITEM_STATE_NONE)
		{
			animation_set->at(this->state)->Render(x, y);
			RenderBoundingBox();
		}

	}

}

void CItem::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (!isEnable)
	{
		l = x;
		t = y;
		r = x + 16;
		b = y + 16;
		
	}
}

void CItem::RanDom(int r)
{
	if (r > 6)
	{
		int random = rand() % 100;

		if (random < 10)
		{
			this->state = ITEM_STATE_ITEM_WHIP;
		}
		else if (random < 20 && random >= 10)
		{
			this->state = ITEM_STATE_ITEM_KNIFE;
		}
		else if (random < 30 && random >= 20)
		{
			this->state = ITEM_STATE_ITEM_AXE;
		}
		else if (random < 40 && random >= 30)
		{
			this->state = ITEM_STATE_ITEM_BOMERANG;
		}
		else if (random < 50 && random >= 40)
		{
			this->state = ITEM_STATE_ITEM_FIRE;
		}
		else if (random < 60 && random >= 50)
		{
			this->state = ITEM_STATE_ITEM_SMALL_HEART;
		}
		else
		{
			this->state = ITEM_STATE_ITEM_BIG_HEART;
		}
	}
	else
		this->state = r;
}

void CItem::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

void CItem::GetPosition(int &_x, int &_y)
{
	x = this->x;
	y = this->y;
}