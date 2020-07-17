#include "Bird.h"

CBird::CBird(int _item)
{
	this->isEnable = true;
	this->isDisplay = true;
	this->isAttacked = false;

	this->blood = 2;

	this->timeChangeState = 0;

	this->nx = -1;
	this->ny = 1;

	whipEffect = new CWhipEffect();

	dieEffect = new CDieEffect();

	SetState(STATE_BIRD_STAND);

	item = CGame::GetInstance()->RandomItem(_item);
}

void CBird::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		left = x;
		top = y;
		right = left + BIRD_WIDTH;
		bottom = top + BIRD_HEIGHT;
	}

}

void CBird::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	if ((this->x - this->xSimon) < DISTANCE_CHANGE_FLY)
	{
		SetState(STATE_BIRD_FLY);
		timeChangeState += dt;

		// set time to change waitting state
		if (timeChangeState > TIME_CHANGE_FLY_WAIT)
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
		if (timeChangeState > TIME_CHANGE_FLY)
		{
			SetState(STATE_BIRD_FLY);
			timeChangeState = 0;
		}
	}

	// set nx of bird
	if (this->xSimon - this->x > DISTANCE_CHANGE_NX)
		nx = 1;
	else if (this->xSimon - this->x < -DISTANCE_CHANGE_NX)
		nx = -1;
	
	// set ny of bird
	if (ySimon - y > DISTANCE_CHANGE_NY)
		ny = 1;
	else if (ySimon - y < DISTANCE_CHANGE_NY_OPPOSITE)
		ny = -1;

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
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;

					if (nx != 0) vx = 0;
					if (ny != 0) vy = 0;
				}
				else
				{
					x += dx;
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
	}
	// render die effect
	if (this->blood < 1)
	{
		dieEffect->SetPosition(x, y);
		dieEffect->Render();
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
			vx = 0.05f;
		}
		else vx = -0.05f;
		
		if (ny > 0)
			vy = 0.05f;
		else
			vy = -0.05f;

		break;
	case STATE_BIRD_FLY_WAIT:
		vx = 0;
		vy = 0;
		break;
	case STATE_ITEM:
		(!isEnable) ? vy = 0 : vy = 0.05f;
		vx = 0;
		break;
	default:
		break;
	}
}


