#include "Bird.h"

CBird::CBird(int item)
{
	this->isEnable = true;
	this->isAttacked = false;

	this->blood = 2;

	this->timeChangeState = 0;

	this->nx = -1;
	this->ny = 1;

	whipEffect = new CWhipEffect();

	dieEffect = new CDieEffect();

	SetState(STATE_BIRD_STAND);

	RanDom(item);
}

void CBird::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		left = x;
		top = y;
		right = left + 16;
		bottom = top + 16;
	}

}

void CBird::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	if ((this->x - this->xSimon) < 100)
	{
		SetState(STATE_BIRD_FLY);
		timeChangeState += dt;

		// set time to change waitting state
		if (timeChangeState > 3000)
		{
			if (this->state == STATE_BIRD_FLY)
			{
				SetState(STATE_BIRD_FLY_WAIT);
			}
		}
	}
	// set time to change fly state from waitting state
	if (this->state == STATE_BIRD_FLY_WAIT)
	{
		timeChangeState += dt;
		if (timeChangeState > 7000)
		{
			SetState(STATE_BIRD_FLY);
			timeChangeState = 0;
		}
	}

	// set nx of bird
	if (this->xSimon - this->x > 30)
		nx = 1;
	else if (this->xSimon - this->x < -30)
		nx = -1;

	// set ny of bird
	if (ySimon - y > 50)
		ny = 1;
	else if (ySimon - y < -10)
		ny = -1;

	// check whip attack
	if (isAttacked)
		whipEffect->Update(dt, coObjects);

	// update die effect
	if (this->blood < 1)
	{
		dieEffect->Update(dt, coObjects);
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
	else
	{
		x += dx;
		y += dy;
	}

}

void CBird::Render()
{
	if (isEnable)
	{
		if (this->state == STATE_BIRD_STAND)
		{
			if (nx > 0)
				ani = ANI_BIRD_STAND_RIGHT;
			else ani = ANI_BIRD_STAND_LEFT;
		}
		else if (this->state == STATE_BIRD_FLY || this->state == STATE_BIRD_FLY_WAIT)
		{
			if (nx > 0)
				ani = ANI_BIRD_FLY_RIGHT;
			else ani = ANI_BIRD_FLY_LEFT;
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

		// render die effect
		if (this->blood < 1)
		{
			dieEffect->SetPosition(x, y);
			dieEffect->Render();
		}
	}

}

void CBird::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_BIRD_STAND:
		vx = 0;
		vy = 0;
		break;
	case STATE_BIRD_FLY:
		if (nx > 0)
		{
			vx = 0.01f;
		}
		else vx = -0.01f;
		
		if (ny > 0)
			vy = 0.01f;
		else
			vy = -0.01f;

		break;
	case STATE_BIRD_FLY_WAIT:
		vx = 0;
		vy = 0;
		break;
	case STATE_ITEM:
		vy = 0.01f;
		vx = 0;
		break;
	default:
		break;
	}
}

void CBird::RanDom(int r)
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
