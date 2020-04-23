#include "Item.h"

CItem::CItem()
{
	isDie = false;
	isEnable = false;
	isItem = false;
}

void CItem::Render()
{
	if (!isEnable)
	{
		if (!isDie && !isItem)
		{
			animation_set->at(0)->Render(x, y);
			RenderBoundingBox();
		}
		else if(isDie && !isItem)
		{
			animation_set->at(1)->Render(x, y + 15);
			isItem = true;
		}
		else
		{
			animation_set->at(2)->Render(x, y + 15);
			//isEnable = true;
		}
			
	}
	
}

void CItem::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y + 32;
}
