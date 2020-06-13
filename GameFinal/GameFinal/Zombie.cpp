#include "Zombie.h"

CZombie::CZombie(int _item)
{
	this->isEnable = true;
	this->isDisplay = true;
	this->isAttacked = false;

	this->width = ZOMBIE_WIDTH;
	this->height = ZOMBIE_HEIGHT;

	this->blood = 1;

	this->nx = -1;

	whipEffect = new CWhipEffect();

	dieEffect = new CDieEffect();

	SetState(STATE_ZOMBIE);

	item = CGame::GetInstance()->RandomItem(_item);
}

void CZombie::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		if (this->state == STATE_ZOMBIE)
		{
			left = x;
			top = y;
			right = left + ZOMBIE_WIDTH;
			bottom = top + ZOMBIE_HEIGHT;
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

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
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

	if (this->x <= POSITION_X_CHANGE_DIRECTION)
	{
		nx = 1;
		SetState(STATE_ZOMBIE);
	}
	else if (this->x > POSITION_X_CHANGE_DIRECTION_OPPOSITE)
	{
		nx = -1;
		SetState(STATE_ZOMBIE);
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
			x += dx;

		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CZombie::Render()
{
	if (isEnable)
	{
		if (this->state == STATE_ZOMBIE)
		{
			if (nx > 0)
				ani = ANI_ZOMBIE_RIGHT;
			else ani = ANI_ZOMBIE_LEFT;
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

void CZombie::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_ZOMBIE:
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


