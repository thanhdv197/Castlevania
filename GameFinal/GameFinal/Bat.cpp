#include "Bat.h"

CBat::CBat(int _item)
{
	this->isEnable = true;
	this->isDisplay = true;
	this->isAttacked = false;

	this->blood = 1;

	this->nx = 1;

	whipEffect = new CWhipEffect();

	dieEffect = new CDieEffect();

	SetState(STATE_BAT_STAND);

	item = CGame::GetInstance()->RandomItem(_item);

	timeDown = 0;

}

void CBat::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		left = x;
		top = y;
		right = left + BAT_WIDTH;
		bottom = top + BAT_HEIGHT;
	}

}

void CBat::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	// check simon position and change state bat
	if (abs(this->x - this->xSimon) < DISTANCE_X && abs(this->y - this->ySimon) < DISTANCE_Y)
	{
		if(this->state == STATE_BAT_STAND)
			(this->x < this->xSimon) ? nx = 1 : nx = -1;

		SetState(STATE_BAT_DOWN);
	}

	// change state to fly 
	if (this->state == STATE_BAT_DOWN)
	{
		timeDown += dt;
		if (timeDown > TIMEDOWN)
			SetState(STATE_BAT_FLY);
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

			/*x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			if (nx != 0) vx = 0;
			if (ny != 0) vy = 0;*/

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

void CBat::Render()
{
	if (isEnable)
	{
		if (this->state == STATE_BAT_STAND)
		{
			ani = ANI_BAT_STAND;
		}
		else if (this->state == STATE_BAT_DOWN || this->state == STATE_BAT_FLY)
		{
			if (nx > 0)
				ani = ANI_BAT_FLY_RIGHT;
			else ani = ANI_BAT_FLY_LEFT;
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

void CBat::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_BAT_FLY:
		if (nx > 0)
		{
			vx = 0.1f;
		}
		else vx = -0.1f;
		vy = 0;
		break;
	case STATE_BAT_STAND:
		vx = 0;
		vy = 0;
		break;
	case STATE_BAT_DOWN:
		if (nx > 0)
		{
			vx = 0.05f;
		}
		else vx = -0.05f;
		vy = 0.1f;
		break;
	case STATE_ITEM:
		(!isEnable) ? vy = 0 : vy = 0.05f;
		vx = 0;
		break;
	default:
		break;
	}
}


