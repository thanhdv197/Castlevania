#include "Boss.h"

CBoss::CBoss()
{
	this->isEnable = true;
	this->isAttacked = false;

	this->blood = 16;

	this->timeChangeState = 0;
	this->timeDead = 0;

	this->nx = -1;
	this->ny = 1;

	whipEffect = new CWhipEffect();

	SetState(STATE_BOSS_STAND);
}

void CBoss::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		if (this->state == STATE_BOSS_FLY || this->state == STATE_BOSS_FLY_WAIT)
		{
			left = x;
			top = y;
			right = left + 48;
			bottom = top + 24;
		}
		else if (this->state == STATE_BOSS_STAND)
		{
			left = x;
			top = y;
			right = left + 16;
			bottom = top + 16;
		}
	}

}

void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	// set distance to change state of boss
	if ((this->xSimon - this->x) > 40)
	{
		SetState(STATE_BOSS_FLY);
	}

	if (this->state != STATE_BOSS_STAND)
	{
		timeChangeState += dt;
	}

	if (this->state == STATE_BOSS_FLY)
	{
		// set time to change waitting state
		if (timeChangeState > 2000)
		{
			SetState(STATE_BOSS_FLY_WAIT);
			timeChangeState = 0;
		}
	}
	else if (this->state == STATE_BOSS_FLY_WAIT)
	{
		if (timeChangeState > 2000)
		{
			// set nx of bird
			if (this->xSimon < this->x)
				nx = -1;
			else
				nx = 1;

			// set ny of bird
			if (ySimon > y)
				ny = 1;
			else
				ny = -1;

			SetState(STATE_BOSS_ATTACK);

			timeChangeState = 0;
		}
	}
	else if (this->state == STATE_BOSS_ATTACK)
	{
		if (timeChangeState > 2000)
		{
			SetState(STATE_BOSS_FLY_WAIT);
			timeChangeState = 0;
		}
	}

	// check whip attack
	if (isAttacked)
		whipEffect->Update(dt, coObjects);

	// update die effect
	if (this->blood < 1)
	{
		SetState(STATE_BOSS_DIE);

		timeDead += dt;
		if (timeDead > 1000)
			isEnable = false;
	}

	x += dx;
	y += dy;

}

void CBoss::Render()
{
	if (isEnable)
	{
		if (this->state == STATE_BOSS_STAND)
		{
			ani = ANI_BOSS_STAND;
		}
		else if (this->state == STATE_BOSS_FLY || this->state == STATE_BOSS_FLY_WAIT)
		{
			ani = ANI_BOSS_FLY;
		}
		else if (this->state == STATE_BOSS_DIE)
		{
			ani = ANI_BOSS_DIE;
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

}

void CBoss::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_BOSS_STAND:
		vx = 0;
		vy = 0;
		break;
	case STATE_BOSS_FLY:
		if (nx > 0)
		{
			vx = 0.05f;
		}
		else vx = -0.05f;

		if (ny > 0)
			vy = 0.03f;
		else
			vy = -0.03f;
		break;
	case STATE_BOSS_FLY_WAIT:
		vx = 0;
		vy = 0;
		break;
	case STATE_BOSS_DIE:
		vy = 0;
		vx = 0;
		break;
	case STATE_BOSS_ATTACK:
		if (nx > 0)
		{
			vx = 0.1f;
		}
		else vx = -0.1f;

		if (ny > 0)
			vy = 0;
		else
			vy = -0;
		break;
	default:
		break;
	}
}

