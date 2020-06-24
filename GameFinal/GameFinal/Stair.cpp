#include "Stair.h"

CStair::CStair(int _nx, int _state)
{
	SetState(_state);
	nx = _nx;
	vx = 0;
	vy = 0;

	SetAnimationSet(CAnimationSets::GetInstance()->Get(43));
}

void CStair::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;

}

void CStair::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
}

void CStair::Render()
{
	if (this->state == STATE_1)
	{
		(nx > 0) ? ani = ANI_RIGHT_1 : ani = ANI_LEFT_1;
	}
	else if (this->state == STATE_2)
	{
		(nx > 0) ? ani = ANI_RIGHT_2 : ani = ANI_LEFT_2;
	}
	else
	{
		(nx > 0) ? ani = ANI_RIGHT_3 : ani = ANI_LEFT_3;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CStair::SetState(int state)
{
	CGameObject::SetState(state);
}
