#include "Weapon.h"

CWeapon::CWeapon(int state) : CGameObject()
{
	this->isEnable = true;

	this->state = state;

	this->timeAttack = 0.0f;

	SetAnimationSet(CAnimationSets::GetInstance()->Get(48));
}

CWeapon::CWeapon(float x, float y, int nx, int state) : CGameObject()
{
	this->x = x;
	this->y = y;
	this->nx = nx;

	this->isEnable = true;

	this->state = state;

	SetAnimationSet(CAnimationSets::GetInstance()->Get(48));
}

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (isEnable)
	{
		timeAttack += dt;

		if (timeAttack > 1000)
		{
			this->isEnable = false;
		}
		else
		{
			SetState(state);

			if (timeAttack < 500)
			{
				vy = -vy;
			}
			else vy += vy;

			x += dx;
			y += dy;
		}
		
	}

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CTorch*>(coObjects->at(i))) {
			CTorch* torch = dynamic_cast<CTorch*>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			torch->GetBoundingBox(l2, t2, r2, b2);

			if (t1 <= b2 && b1 >= t2 && l1 <= r2 && r1 >= l2)
			{
				torch->isAttacking = true;
				torch->SetState(STATE_ITEM);
			}
		}

	}
}

void CWeapon::Render()
{
	if (isEnable)
	{
		if (this->state == WEAPON_STATE_NONE)
		{

		}
		else
		{
			if (this->state == WEAPON_STATE_KNIFE)
			{
				if (nx > 0)
				{
					ani = WEAPON_ANI_KNIFE_RIGHT;
				}
				else
				{
					ani = WEAPON_ANI_KNIFE_LEFT;
				}
			}
			else if (this->state == WEAPON_STATE_AXE)
			{
				ani = WEAPON_ANI_AXE;
			}

			animation_set->at(ani)->Render(x, y);
			RenderBoundingBox();
		}
	}
	
}

void CWeapon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (this->state != WEAPON_STATE_NONE)
	{
		left = x;
		top = y;
		right = left + 16;
		bottom = top + 16;
	}

}

void CWeapon::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CWeapon::GetPosition(float &x, float &y) {
	x = this->x;
	y = this->y;
}

void CWeapon::SetDirection(int nx)
{
	this->nx = nx;
}

int CWeapon::GetDirection()
{
	return this->nx;
}

void CWeapon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case WEAPON_STATE_KNIFE:
		if (nx > 0)
		{
			vx = WEAPON_FLY_SPEED;
		}
		else vx = -WEAPON_FLY_SPEED;
		vy = 0;
		break;
	case WEAPON_STATE_AXE:
		if (nx > 0)
		{
			vx = WEAPON_FLY_SPEED;
		}
		else vx = -WEAPON_FLY_SPEED;
		vy = 0.04f;
		break;
	default:
		break;
	}
}
