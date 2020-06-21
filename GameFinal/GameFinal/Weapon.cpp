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
	this->isBurning = false;
	this->state = state;

	SetAnimationSet(CAnimationSets::GetInstance()->Get(48));
}

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	CGame * game = CGame::GetInstance();

	if (isEnable)
	{
		timeAttack += dt;

		if (this->state == WEAPON_STATE_BOMERANG)
		{
			if (timeAttack > TIME_ENABLE_BOMERANG_FIRE)
			{
				this->isEnable = false;
				this->isAttack = false;
			}
			else
			{
				SetState(state);

				if (timeAttack > TIME_CHANGE_DIRECTION_BOMERANG)
				{
					vx = -vx;
				}
			}
		}
		else if (this->state == WEAPON_STATE_FIRE)
		{
			if (timeAttack > TIME_ENABLE_BOMERANG_FIRE)
			{
				this->isEnable = false;
				this->isAttack = false;
			}
			else
			{
				SetState(state);

				if (timeAttack < TIME_DOWN_FIRE)
				{
					vy = -vy;
				}
				else vy += vy;
			}
		}
		else
		{
			if (timeAttack > TIME_ENABLE_KNIFE_AXE)
			{
				this->isEnable = false;
				this->isAttack = false;
			}
			else
			{
				SetState(state);

				if (timeAttack < TIME_DOWN_AXE)
				{
					vy = -vy;
				}
				else vy += vy;
			}
		}

		x += dx;
		y += dy;
		
	}

	if (isAttack)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (dynamic_cast<CTorch*>(coObjects->at(i))) {
				CTorch* torch = dynamic_cast<CTorch*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				torch->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					torch->isAttacked = true;
					torch->SetBlood(GetDame());
					this->isAttack = false;
				}
			}
			else if (dynamic_cast<CCandle*>(coObjects->at(i))) {
				CCandle* candle = dynamic_cast<CCandle*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				candle->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					candle->isAttacked = true;
					candle->SetBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CArmy*>(coObjects->at(i))) {
				CArmy* army = dynamic_cast<CArmy*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				army->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					army->isAttacked = true;
					army->LostBlood(GetDame());
					this->isAttack = false;
				}
			}
			else if (dynamic_cast<CZombie*>(coObjects->at(i))) {
				CZombie* zombie = dynamic_cast<CZombie*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				zombie->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					zombie->isAttacked = true;
					zombie->LostBlood(GetDame());
					this->isAttack = false;
				}
			}
			else if (dynamic_cast<CBat*>(coObjects->at(i))) {
				CBat* bat = dynamic_cast<CBat*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				bat->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					bat->isAttacked = true;
					bat->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CFlea*>(coObjects->at(i))) {
				CFlea* flea = dynamic_cast<CFlea*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				flea->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					flea->isAttacked = true;
					flea->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CBird*>(coObjects->at(i))) {
				CBird* bird = dynamic_cast<CBird*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				bird->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					bird->isAttacked = true;
					bird->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CToad*>(coObjects->at(i))) {
				CToad* toad = dynamic_cast<CToad*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				toad->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					toad->isAttacked = true;
					toad->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CSkeleton*>(coObjects->at(i))) {
				CSkeleton* skeleton = dynamic_cast<CSkeleton*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				skeleton->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					skeleton->isAttacked = true;
					skeleton->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CBoss*>(coObjects->at(i))) {
				CBoss* boss = dynamic_cast<CBoss*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				boss->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					boss->isAttacked = true;
					boss->LostBlood(GetDame());
					isAttack = false;
				}
			}
			else if (dynamic_cast<CBricks*>(coObjects->at(i))) {
				CBricks* bricks = dynamic_cast<CBricks*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				bricks->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					isBurning = true;
				}
			}
		}
	}
	
}

void CWeapon::Render()
{
	if (isEnable)
	{
		if (this->state != WEAPON_STATE_NONE)
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
			else if (this->state == WEAPON_STATE_BOMERANG)
			{
				ani = WEAPON_ANI_BOMERANG;
			}
			else if (this->state == WEAPON_STATE_FIRE)
			{
				(isBurning) ? ani = WEAPON_ANI_FIRE_BURN : ani = WEAPON_ANI_FIRE;
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
		(nx > 0) ? vx = WEAPON_FLY_SPEED : vx = -WEAPON_FLY_SPEED;
		vy = 0;
		this->dame = 1;
		break;
	case WEAPON_STATE_AXE:
		(nx > 0) ? vx = WEAPON_FLY_SPEED : vx = -WEAPON_FLY_SPEED;
		vy = 0.04f;
		this->dame = 2;
		break;
	case WEAPON_STATE_BOMERANG:
		(nx > 0) ? vx = WEAPON_FLY_SPEED : vx = -WEAPON_FLY_SPEED;
		vy = 0;
		this->dame = 2;
		break;
	case WEAPON_STATE_FIRE:
		if (isBurning)
			vx = 0;
		else
			(nx > 0) ? vx = WEAPON_FLY_SPEED : vx = -WEAPON_FLY_SPEED;
		(isBurning) ? vy = 0 : vy = 0.05f;
		this->dame = 1;
		break;
	default:
		break;
	}
}
