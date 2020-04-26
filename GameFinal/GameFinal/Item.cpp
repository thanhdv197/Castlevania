#include "Item.h"

CItem::CItem(int itemType)
{
	//isDead = false;
	isEnable = false;

	RanDom(itemType);

	SetState(ITEM_STATE_NONE);
}

void CItem::Render()
{
	if (!isEnable)
	{
		if (this->state == ITEM_STATE_NONE)
		{
			animation_set->at(0)->Render(x, y);
			RenderBoundingBox();
		}
		else
		{
			animation_set->at(randomItem)->Render(x, y + 15);
			
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
		b = y + 32;
		
	}
}

void CItem::SetState(int state)
{
	switch (state)
	{
	case ITEM_STATE_NONE:
		this->state = ITEM_STATE_NONE;
		break;
	case ITEM_STATE_ITEM_WHIP:
		this->state = ITEM_STATE_ITEM_WHIP;
		break;
	case ITEM_STATE_ITEM_KNIFE:
		this->state = ITEM_STATE_ITEM_KNIFE;
		break;
	case ITEM_STATE_ITEM_AXE:
		this->state = ITEM_STATE_ITEM_AXE;
		break;
	case ITEM_STATE_ITEM_BOMERANG:
		this->state = ITEM_STATE_ITEM_BOMERANG;
		break;
	case ITEM_STATE_ITEM_SMALL_HEART:
		this->state = ITEM_STATE_ITEM_SMALL_HEART;
		break;
	case ITEM_STATE_ITEM_BIG_HEART:
		this->state = ITEM_STATE_ITEM_BIG_HEART;
		break;
	case ITEM_STATE_ITEM_FIRE:
		this->state = ITEM_STATE_ITEM_FIRE;
		break;
	case ITEM_STATE_DESTROY:
		this->state = randomItem;
		break;
	default:
		break;
	}
}

void CItem::RanDom(int r)
{
	if (r < 2)
	{
		int random = rand() % 100;

		if (random < 10)
		{
			this->randomItem = 2;
		}
		else if (random < 20 && random >= 10)
		{
			this->randomItem = 3;
		}
		else if (random < 30 && random >= 20)
		{
			this->randomItem = 4;
		}
		else if (random < 40 && random >= 30)
		{
			this->randomItem = 5;
		}
		else if (random < 50 && random >= 40)
		{
			this->randomItem = 8;
		}
		else if (random < 60 && random >= 50)
		{
			this->randomItem = 7;
		}
		else
		{
			this->randomItem = 6;
		}
	}
	else
		this->randomItem = r;
}
