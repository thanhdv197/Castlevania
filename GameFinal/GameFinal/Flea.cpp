#include "Flea.h"

CFlea::CFlea(int _item)
{
	this->isEnable = true;
	this->isDisplay = true;
	this->isAttacked = false;

	this->blood = 2;

	this->nx = -1;

	whipEffect = new CWhipEffect();

	dieEffect = new CDieEffect();

	SetState(STATE_FLEA_WAIT);

	item = CGame::GetInstance()->RandomItem(_item);

}

void CFlea::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		left = x;
		top = y;
		right = left + FLEA_WIDTH;
		bottom = top + FLEA_HEIGHT;
	}

}

void CFlea::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	// check simon position and change state bat
	if ((this->x - this->xSimon) > DISTANCE_CHANGE_STATE && this->x > this->xSimon)
	{
		SetState(STATE_FLEA_FLY);

		if (GetBlood() > 0)
			isEnable = true;
	}

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

	if (state == STATE_ITEM)
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
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;

					if (nx != 0) vx = 0;
					if (ny != 0) vy = 0;

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

void CFlea::Render()
{
	if (isEnable)
	{
		if (this->state != STATE_ITEM)
		{
			if (nx > 0)
				ani = ANI_FLEA_RIGHT;
			else ani = ANI_FLEA_LEFT;
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

void CFlea::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_FLEA_FLY:
		if (nx > 0)
		{
			vx = 0.04f;
		}
		else vx = -0.04f;
		vy = -0.01f;
		break;
	case STATE_FLEA_WAIT:
		vx = 0;
		vy = 0;
		isEnable = false;
		break;
	case STATE_ITEM:
		vy = 0.01f;
		vx = 0;
		break;
	default:
		break;
	}
}


