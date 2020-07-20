#include "Skeleton.h"

CSkeleton::CSkeleton(int _item)
{
	this->isEnable = true;
	this->isDisplay = true;
	this->isAttacked = false;
	this->isJump = false;

	this->width = SKELETON_WIDTH;
	this->height = SKELETON_HEIGHT;

	this->blood = 2;

	this->nx = 1;

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

	if (this->state == STATE_SKELETON_STAND)
	{
		start_x = x;
		start_y = y;
	}

	// set direction of skeleton
	(this->x < this->xSimon) ? nx = 1 : nx = -1;

	// set bone
	if (!bone->isEnable)
	{
		bone->SetState(STATE_NONE);
		bone->ResetTimeThrow();
		bone->SetPosition(x, y);
		bone->SetNx(nx);
	}
	
	// set jump
	if (this->state == STATE_SKELETON_JUMP)
	{	
		if (abs(y-start_y) > DISTANCE_Y_JUMP_DOWN)
		{
			isJump = false;
			SetState(STATE_SKELETON_JUMP);
		}
			
	}

	// set state of skeleton
	if (abs(this->x - this->xSimon) < DISTANCE_STATE_JUMP)
	{
		(this->y > this->ySimon) ? SetState(STATE_THROW) : SetState(STATE_SKELETON_JUMP);
	}

	if (this->state == STATE_THROW)
	{
		bone->Update(dt, coObjects);

		if (this->state == STATE_THROW)
		{
			timeWalkThrow += dt;
			if (timeWalkThrow > TIME_WALK_THROW)
			{
				vx = -vx;
				if (timeWalkThrow > TIME_WALK_THROW_OPPOSITE)
				{
					timeWalkThrow = 0;
					vx = -vx;
				}
			}
		}
	}

	// jump opposite
	if (isOpposite) vx = -vx;

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
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;

				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;

				isJump = true;

				if (abs(this->x - this->start_x) > 20)
					isOpposite = true;
				else isOpposite = false;
				
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
		isOpposite = false;
		vx = 0;
		vy = 0.1f;
		break;
	case STATE_SKELETON_JUMP:
		bone->isEnable = false;
		(nx > 0) ? vx = 0.06f : vx = -0.06f;
		(isJump==true) ? vy = -0.1f : vy = 0.1f;
		break;
	case STATE_ITEM:
		bone->isEnable = false;
		(!isEnable) ? vy = 0 : vy = 0.05f;
		vx = 0;
		break;
	case STATE_THROW:
		(nx > 0) ? vx = 0.01f : vx = -0.01f;
		vy = 0.1f;
		bone->SetState(STATE_THROW);
		break;
	default:
		break;
	}
}


