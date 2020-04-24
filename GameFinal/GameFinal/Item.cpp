#include "Item.h"

CItem::CItem()
{
	//isDead = false;
	isEnable = false;

	randomItem = 2 + rand() % (3 + 1 - 2);

	SetState(ITEM_STATE_NONE);
}

void CItem::Render()
{
	if (!isEnable)
	{
		/*switch (state)
		{
		case ITEM_STATE_NONE:
			animation_set->at(0)->Render(x, y);
			RenderBoundingBox();
			break;
		case ITEM_STATE_ITEM_WHIP:
			animation_set->at(2)->Render(x, y + 15);
			break;
		default:
			break;
		}*/

		if (this->state == ITEM_STATE_NONE)
		{
			animation_set->at(0)->Render(x, y);
			RenderBoundingBox();
		}
		else
		{
			if (randomItem == 2)
			{
				SetState(ITEM_STATE_ITEM_WHIP);
			}
			else if (randomItem == 3)
			{
				SetState(ITEM_STATE_ITEM_KNIFE);
			}
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
	case ITEM_STATE_DESTROY:
		this->state = ITEM_STATE_DESTROY;
		break;
	default:
		break;
	}
}
