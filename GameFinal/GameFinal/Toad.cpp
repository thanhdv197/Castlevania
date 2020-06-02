#include "Toad.h"

CToad::CToad(int item)
{
	this->isEnable = true;
	this->isDisplay = true;
	this->isAttacked = false;

	this->blood = 1;

	this->nx = 1;

	this->timeJump = 0;

	whipEffect = new CWhipEffect();

	dieEffect = new CDieEffect();

	SetState(STATE_TOAD_STAND);

	RanDom(item);
}

void CToad::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		left = x;
		top = y;
		right = left + 16;
		bottom = top + 16;
	}

}

void CToad::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	// set state of toad
	if (this->xSimon - this->x > 80 && this->xSimon - this->x < 100)
	{
		SetState(STATE_TOAD_JUMP);
	}	

	// set time jump
	if (this->state == STATE_TOAD_JUMP)
	{
		timeJump += dt;

		if (timeJump > 400)
		{
			SetState(STATE_TOAD_DOWN);
		}
	}
	// set time down
	if (this->state == STATE_TOAD_DOWN)
	{
		timeJump += dt;
		if (timeJump > 1200)
		{
			SetState(STATE_TOAD_STAND);
			timeJump = 0;
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
				if (e->ny < 0)
				{
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;

					if (nx != 0) vx = 0;
					if (ny != 0) vy = 0;
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
		vy = -0.03f;
		break;
	case STATE_TOAD_DOWN:
		if (nx > 0)
			vx = 0.04f;
		else vx = -0.04f;
		vy = 0.1f;
		break;
	case STATE_ITEM:
		vy = 0.05f;
		vx = 0;
		break;
	default:
		break;
	}
}

void CToad::RanDom(int r)
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
