#include "Candle.h"

CCandle::CCandle(int _item)
{
	this->isEnable = true;
	this->isDisplay = true;
	this->isAttacked = false;

	this->blood = 1;

	whipEffect = new CWhipEffect();

	dieEffect = new CDieEffect();

	SetState(STATE_CANDLE);

	item = CGame::GetInstance()->RandomItem(_item);
}

void CCandle::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		if (this->state == STATE_CANDLE)
		{
			left = x;
			top = y;
			right = left + CANDLE_WIDTH;
			bottom = top + CANDLE_HEIGHT;
		}
		else
		{
			left = x;
			top = y;
			right = left + ITEM_WIDTH;
			bottom = top + ITEM_HEIGHT;
		}
	}
	
}

void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	// check whip attack
	if (isAttacked)
		whipEffect->Update(dt, coObjects);

	// set die effect
	if (this->blood < 1)
	{
		dieEffect->Update(dt, coObjects);

		if (dieEffect->isEnable == true)
			isEnable = false;
		else isEnable = true;

		if (!isDisplay) isEnable = false;

		SetState(STATE_ITEM);
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
		if (isAttacked)
		{
			whipEffect->SetPosition(x, y);
			whipEffect->Render();
		}
	}
	// render die effect
	if (this->blood < 1)
	{
		dieEffect->SetPosition(x, y);
		dieEffect->Render();
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
		vy = 0.05f;
		break;
	default:
		break;
	}
}

