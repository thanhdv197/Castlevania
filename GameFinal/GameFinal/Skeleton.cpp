#include "Skeleton.h"

CSkeleton::CSkeleton(int item)
{
	this->isEnable = true;
	this->isDisplay = true;
	this->isAttacked = false;

	this->blood = 2;

	this->nx = -1;

	this->timeChangeDirection = 0;

	whipEffect = new CWhipEffect();

	dieEffect = new CDieEffect();

	bone = new CBone();

	SetState(STATE_SKELETON_STAND);

	RanDom(item);
}

void CSkeleton::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		if (this->state != STATE_ITEM)
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

void CSkeleton::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	// set direction of skeleton
	if (this->x < this->xSimon)
		nx = 1;
	else nx = -1;

	// set state of skeleton
	if (abs(this->x - this->xSimon) < 70)
	{
		SetState(STATE_SKELETON_WALK);

		if (abs(this->x - this->xSimon) < 40)
		{
			SetState(STATE_THROW);
			bone->Update(dt, coObjects);
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

	// set collision
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

void CSkeleton::Render()
{
	if (isEnable)
	{
		if (this->state != STATE_ITEM)
		{
			if (nx > 0)
				ani = ANI_SKELETON_RIGHT;
			else ani = ANI_SKELETON_LEFT;
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

		// render bone
		if (bone)
		{
			bone->SetPosition(x, y);
			bone->Render();
		}
	}
	// render die effect
	if (this->blood < 1)
	{
		dieEffect->SetPosition(x, y);
		dieEffect->Render();
	}
}

void CSkeleton::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_SKELETON_STAND:
		vx = 0;
		vy = 0.1f;
		break;
	case STATE_SKELETON_WALK:
		if (nx > 0) vx = 0.05f;
		else vx = -0.05f;
		vy = 0.1f;
		break;
	case STATE_ITEM:
		vy = 0.1f;
		vx = 0;
		break;
	case STATE_THROW:
		vx = 0;
		vy = 0;
		bone->SetState(STATE_THROW);
		break;
	default:
		break;
	}
}

void CSkeleton::RanDom(int r)
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
