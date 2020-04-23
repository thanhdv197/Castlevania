#include "Whip.h"

CWhip::CWhip(float x, float y, int nx) : CGameObject()
{
	x = x;
	y = y;
	nx = nx;
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// No collision occured, proceed normally
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

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CItem *>(e->obj)) // if e->obj is Goomba 
			{
				CItem *item = dynamic_cast<CItem *>(e->obj);

				item->isDie = true;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CWhip::Render()
{
	SetAnimationSet(CAnimationSets::GetInstance()->Get(49));
	if (nx > 0)
	{
		animation_set->at(0)->Render(x, y);
	}
	else 
		animation_set->at(1)->Render(x, y);
	RenderBoundingBox();
}

void CWhip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + 54;
	bottom = top + 16;
}

void CWhip::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CWhip::SetDirection(int nx)
{
	this->nx = nx;
}

int CWhip::GetDirection()
{
	return this->nx;
}