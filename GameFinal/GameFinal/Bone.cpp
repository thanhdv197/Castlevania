#include "Bone.h"

CBone::CBone()
{
	this->isEnable = true;
	SetState(STATE_NONE);
	this->timeThrow = 0;

	this->nx = 1;
	this->ny = -1;

	SetAnimationSet(CAnimationSets::GetInstance()->Get(33));
}

void CBone::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isEnable)
	{
		left = x;
		top = y;
		right = left + BONE_WIDTH;
		bottom = top + BONE_HEIGHT;
	}

}

void CBone::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (state == STATE_THROW)
	{
		x += dx;
		y += dy;

		timeThrow += dt;

		if (timeThrow > TIME_UP)
		{
			ny = 1;

			if (timeThrow > TIME_DOWN)
			{
				SetState(STATE_NONE);
				timeThrow = 0;
			}
		}
	}

}

void CBone::Render()
{
	if (isEnable)
	{
		animation_set->at(ANI_THROW)->Render(x, y);
		RenderBoundingBox();
	}
	
}

void CBone::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_NONE:
		isEnable = false;
		vx = 0;
		vy = 0;
		ny = -1;
		break;
	case STATE_THROW:
		isEnable = true;
		if (nx > 0) vx = 0.05f;
		else vx = -0.05f;
		if (ny > 0) vy = 0.05f;
		else vy = -0.05f;
		break;
	default:
		break;
	}
}

void CBone::SetPosition(float x, float y)
{
	if (this->state == STATE_NONE)
	{
		this->x = x;
		this->y = y;
	}
}