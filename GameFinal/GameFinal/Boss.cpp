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
			bottom = top + 32;
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

	if ((this->xSimon - this->x) > 50)
	{
		SetState(STATE_BOSS_FLY);
		timeChangeState += dt;

		// set time to change waitting state
		if (timeChangeState > 3000)
		{
			if (this->state == STATE_BOSS_FLY)
			{
				SetState(STATE_BOSS_FLY_WAIT);
			}
		}
	}
	// set time to change fly state from waitting state
	if (this->state == STATE_BOSS_FLY_WAIT)
	{
		timeChangeState += dt;
		if (timeChangeState > 7000)
		{
			SetState(STATE_BOSS_FLY);
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
		SetState(STATE_BOSS_DIE);

		if (this->state == STATE_BOSS_DIE)
		{
			timeDead += dt;
			if (timeDead > 300)
				isEnable = true;
		}
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
			vx = 0.01f;
		}
		else vx = -0.01f;

		if (ny > 0)
			vy = 0.01f;
		else
			vy = -0.01f;

		break;
	case STATE_BOSS_FLY_WAIT:
		vx = 0;
		vy = 0;
		break;
	case STATE_BOSS_DIE:
		vy = 0;
		vx = 0;
		break;
	default:
		break;
	}
}

