#include "Skeleton.h"

CSkeleton::CSkeleton(int _item)
{
	this->isEnable = true;
	this->isDisplay = true;
	this->isAttacked = false;

	this->width = SKELETON_WIDTH;
	this->height = SKELETON_HEIGHT;

	this->blood = 2;

	this->nx = 1;

	this->timeChangeDirection = 0;

	whipEffect = new CWhipEffect();

	dieEffect = new CDieEffect();

	bone = new CBone();

	SetState(STATE_SKELETON_STAND);

	item = CGame::GetInstance()->RandomItem(_item);
}

void CSkeleton::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		if (this->state != STATE_ITEM)
		{
			left = x;
			top = y;
			right = left + SKELETON_WIDTH;
			bottom = top + SKELETON_HEIGHT;
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

		if (abs(this->x - this->xSimon) < 50 && abs(this->ySimon - this->y) < 20)
		{
			SetState(STATE_THROW);
			bone->Update(dt, coObjects);
		}
	}
	// set bone position again
	if (bone->state == STATE_NONE)
	{
		bone->SetPosition(x, y);
		bone->SetNx(nx);
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
		bone->isEnable = false;
		bone->SetPosition(x, y);
		vx = 0;
		vy = 0.1f;
		break;
	case STATE_SKELETON_WALK:
		bone->isEnable = false;
		bone->SetPosition(x, y);
		if (nx > 0) vx = 0.05f;
		else vx = -0.05f;
		vy = 0.1f;
		break;
	case STATE_ITEM:
		bone->isEnable = false;
		(!isEnable) ? vy = 0 : vy = 0.05f;
		vx = 0;
		break;
	case STATE_THROW:
		vx = 0;
		vy = 0.1f;
		bone->SetState(STATE_THROW);
		break;
	default:
		break;
	}
}


