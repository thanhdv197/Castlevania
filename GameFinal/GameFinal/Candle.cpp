#include "Candle.h"

CCandle::CCandle(int item)
{
	this->isEnable = true;
	this->isAttacking = false;

	whipEffect = new CWhipEffect();
	this->timeHit = 0;

	SetState(STATE_CANDLE);

	RanDom(item);
}

void CCandle::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		if (this->state == STATE_CANDLE)
		{
			left = x;
			top = y;
			right = left + 8;
			bottom = top + 8;
		}
		else
		{
			left = x;
			top = y;
			right = left + 16;
			bottom = top + 16;
		}
	}
	
}

void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	// set time hit
	if (isAttacking)
	{
		timeHit += dt;
		if (timeHit > 100)
			isAttacking = false;
	}

	// set collision of item state
	if (this->state == STATE_ITEM)
	{
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<CBricks*>(e->obj))
				{
					if (e->ny < 0)
					{
						x += min_tx * dx + nx * 0.4f;
						y += min_ty * dy + ny * 0.4f;

						if (nx != 0) vx = 0;
						if (ny != 0) vy = 0;
					}
				}

			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}

}

void CCandle::Render()
{
	if (isEnable)
	{
		if (this->state == STATE_CANDLE)
		{
			ani = ANI_CANDLE;
		}
		else
		{
			ani = item;
		}

		animation_set->at(ani)->Render(x, y);
		RenderBoundingBox();

		// render hit attack of whip 
		if (isAttacking)
		{
			whipEffect->SetPosition(x, y);
			whipEffect->Render();
		}
	}
	
}

void CCandle::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_CANDLE:
		vy = 0;
		break;
	case STATE_ITEM:
		vy = 0.01f;
		break;
	default:
		break;
	}
}

void CCandle::RanDom(int r)
{
	if (r > 6)
	{
		int random = rand() % 100;

		if (random < 10)
		{
			this->item = ANI_ITEM_WHIP;
		}
		else if (random < 20 && random >= 10)
		{
			this->item = ANI_ITEM_KNIFE;
		}
		else if (random < 30 && random >= 20)
		{
			this->item = ANI_ITEM_AXE;
		}
		else if (random < 40 && random >= 30)
		{
			this->item = ANI_ITEM_BOMERANG;
		}
		else if (random < 50 && random >= 40)
		{
			this->item = ANI_ITEM_FIRE;
		}
		else if (random < 60 && random >= 50)
		{
			this->item = ANI_ITEM_SMALL_HEART;
		}
		else
		{
			this->item = ANI_ITEM_BIG_HEART;
		}
	}
	else
	{
		this->item = r;
	}
		
}