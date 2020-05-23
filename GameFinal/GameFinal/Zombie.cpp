#include "Zombie.h"

CZombie::CZombie(int item)
{
	this->isEnable = true;
	this->isAttacked = false;

	this->blood = 1;

	this->nx = -1;

	whipEffect = new CWhipEffect();

	dieEffect = new CDieEffect();

	SetState(STATE_ZOMBIE);

	RanDom(item);
}

void CZombie::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		if (this->state == STATE_ZOMBIE)
		{
			left = x;
			top = y;
			right = left + 16;
			bottom = top + 32;
		}
		else
		{
			left = x;
			top = y;
			right = left + 16;
			bottom = top + 16;
		}
	}

}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	// check whip attack
	if (isAttacked)
		whipEffect->Update(dt, coObjects);

	// update die effect
	if (this->blood < 1)
	{
		dieEffect->Update(dt, coObjects);
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

		// render die effect
		if (this->blood < 1)
		{
			dieEffect->SetPosition(x, y);
			dieEffect->Render();
		}
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
			vx = 0.02f;
		}
		else vx = -0.02f;
		vy = 0.1f;
		break;
	case STATE_ITEM:
		vy = 0.01f;
		vx = 0;
		break;
	default:
		break;
	}
}

void CZombie::RanDom(int r)
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