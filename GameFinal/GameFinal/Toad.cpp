#include "Toad.h"

CToad::CToad(int _item)
{
	this->isEnable = true;
	this->isDisplay = true;
	this->isAttacked = false;

	this->blood = 1;

	this->nx = 1;

	whipEffect = new CWhipEffect();

	dieEffect = new CDieEffect();

	SetState(STATE_TOAD_STAND);

	item = CGame::GetInstance()->RandomItem(_item);
}

void CToad::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		left = x;
		top = y;
		right = left + TOAD_WIDTH;
		bottom = top + TOAD_HEIGHT;
	}

}

void CToad::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	// set state of toad
	if (this->state == STATE_TOAD_STAND)
	{
		if (abs(this->xSimon - this->x) < DISTANC_CHANGE_STATE_JUMP)
		{
			SetState(STATE_TOAD_JUMP);
		}
	}

	// set time jump
	if (this->state == STATE_TOAD_JUMP)
	{
		if (this->ySimon > this->y + 55)
		{
			SetState(STATE_TOAD_DOWN);
			(this->x < this->xSimon) ? nx = 1 : nx = -1;
		}
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
		float min_tx, min_ty, nx, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBricks*>(e->obj))
			{
				if (this->state == STATE_TOAD_DOWN)
				{
					SetState(STATE_TOAD_JUMP);
				}
				if (vy < 0)
				{
					x += dx;
					y += dy;
				}
			}
			else
			{
				x += dx;
				y += dy;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CToad::Render()
{
	if (isEnable)
	{
		if (this->state == STATE_TOAD_STAND)
		{
			if (nx > 0)
				ani = ANI_TOAD_STAND_RIGHT;
			else ani = ANI_TOAD_STAND_LEFT;
		}
		else if (this->state == STATE_TOAD_JUMP || this->state == STATE_TOAD_DOWN)
		{
			if (nx > 0)
				ani = ANI_TOAD_JUMP_RIGHT;
			else ani = ANI_TOAD_JUMP_LEFT;
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

void CToad::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_TOAD_STAND:
		vx = 0;
		vy = 0.1f;
		break;
	case STATE_TOAD_JUMP:
		if (nx > 0)
			vx = 0.05f;
		else vx = -0.05f;
		vy = -0.1f;
		break;
	case STATE_TOAD_DOWN:
		if (nx > 0)
			vx = 0.05f;
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


