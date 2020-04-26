#include "Weapon.h"

CWeapon::CWeapon(float x, float y, int nx) : CGameObject()
{
	x = x;
	y = y;
	nx = nx;

	this->isEnable = true;

	SetState(WEAPON_STATE_KNIFE);

	SetAnimationSet(CAnimationSets::GetInstance()->Get(48));
}

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;

	/*if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}*/

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CItem*>(coObjects->at(i))) {
			CItem* item = dynamic_cast<CItem*>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			item->GetBoundingBox(l2, t2, r2, b2);

			if (t1 <= b2 && b1 >= t2 && l1 <= r2 && r1 >= l2)
			{
				item->SetState(ITEM_STATE_DESTROY);
				//item->isEnable = true;
			}
		}

	}
}

void CWeapon::Render()
{
	if (isEnable)
	{
		if (nx > 0)
		{
			vx = WEAPON_FLY_SPEED;
			ani = WEAPON_ANI_KNIFE_RIGHT;
		}
		else
		{
			vx = - WEAPON_FLY_SPEED;
			ani = WEAPON_ANI_KNIFE_LEFT;
		}

		animation_set->at(ani)->Render(x, y);
		RenderBoundingBox();
	}
}

void CWeapon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + 16;
	bottom = top + 16;

}

void CWeapon::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CWeapon::SetDirection(int nx)
{
	this->nx = nx;
}

int CWeapon::GetDirection()
{
	return this->nx;
}
