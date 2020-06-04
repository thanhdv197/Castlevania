#include "Boss.h"

CBoss::CBoss()
{
	this->isEnable = true;
	this->isAttacked = false;
	this->isDisplay = true;

	this->width = 48;
	this->height = 24;

	this->blood = 16;

	this->start_x = 580;
	this->start_y = 60;

	this->timeChangeState = 0;
	this->timeDead = 0;

	this->nx = -1;
	this->ny = 1;

	this->isBoss = true;

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
		else if (this->state == STATE_BOSS_STAND || this->state == STATE_GLOBULAR)
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

	if (this->state == STATE_BOSS_STAND)
	{
		// set distance to change state of boss
		if ((this->xSimon - this->x) > 40)
		{
			SetState(STATE_BOSS_FLY);
		}
	}
	else 
	{
		timeChangeState += dt;

		if (this->xSimon < this->x) nx = -1;
		else nx = 1;

		if (ySimon > y) ny = 1;
		else ny = -1;

		if (timeChangeState > 2000)
		{
			if (this->state == STATE_BOSS_FLY)
			{
				SetState(STATE_BOSS_FLY_WAIT);
			}
			else if (this->state == STATE_BOSS_FLY_WAIT)
			{
				if (y >= ySimon && y + 48 < CGame::GetInstance()->GetScreenHeight())
					SetState(STATE_BOSS_ATTACK);
				else
					SetState(STATE_BOSS_FLY);
			}
			else if (this->state == STATE_BOSS_ATTACK)
			{
				SetState(STATE_BOSS_FLY_WAIT);
			}

			timeChangeState = 0;
		}
	}
	
	// change direction when boss touch limit height map
	if (y >= ySimon && y + 48 >= CGame::GetInstance()->GetScreenHeight())
	{
		ny = -1;
		SetState(STATE_BOSS_FLY);
	}

	// change direction when boss touch limit width map
	if (x + 48 >= 768)
	{
		nx = -1;
		SetState(STATE_BOSS_FLY);
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
		{
			isEnable = false;
			SetState(STATE_GLOBULAR);

			if (timeDead > 2000)
			{
				(isDisplay) ? isEnable = true : isEnable = false;
			}
		}
	}

	if (!isEnable)
		Reset();

	if (this->state != STATE_GLOBULAR)
	{
		x += dx;
		y += dy;
	}
	else
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

			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			if (nx != 0) vx = 0;
			if (ny != 0) vy = 0;
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
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
		else if (this->state == STATE_GLOBULAR)
		{
			ani = ANI_GLOBULAR;
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
	case STATE_GLOBULAR:
		vx = 0;
		vy = 0.1f;
		break;
	default:
		break;
	}
}


