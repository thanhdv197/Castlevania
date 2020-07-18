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
	SetState(state);

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
				if (timeAttack > TIME_CHANGE_DIRECTION_BOMERANG)
					(nx > 0) ? vx = -BOMERANG_SPEED : vx = BOMERANG_SPEED;
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
				if (!isBurning)
					vy += FIRE_GRAVITY * dt;
				else
				{
					vy = 0;
					vx = 0;
				}
			}
		}
		else if(this->state == WEAPON_STATE_AXE)
		{
			if (timeAttack > TIME_ENABLE_KNIFE_AXE)
			{
				this->isEnable = false;
				this->isAttack = false;
			}
			
			vy += AXE_GRAVITY *dt;
		}
		else if (this->state == WEAPON_STATE_KNIFE)
		{
			if (timeAttack > TIME_ENABLE_KNIFE_AXE)
			{
				this->isEnable = false;
				this->isAttack = false;
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
					if (this->state != WEAPON_STATE_FIRE)
					{
						torch->isAttacked = true;
						torch->SetBlood(GetDame());
						this->isAttack = false;
					}
					else
					{
						if (isBurning)
						{
							torch->isAttacked = true;
							torch->SetBlood(GetDame());
							this->isAttack = false;
						}
					}
				}
			}
			else if (dynamic_cast<CCandle*>(coObjects->at(i))) {
				CCandle* candle = dynamic_cast<CCandle*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				candle->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					if (this->state != WEAPON_STATE_FIRE)
					{
						candle->isAttacked = true;
						candle->SetBlood(GetDame());
						this->isAttack = false;
					}
					else
					{
						if (isBurning)
						{
							candle->isAttacked = true;
							candle->SetBlood(GetDame());
							this->isAttack = false;
						}
					}
				}
			}
			else if (dynamic_cast<CArmy*>(coObjects->at(i))) {
				CArmy* army = dynamic_cast<CArmy*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				army->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					if (this->state != WEAPON_STATE_FIRE)
					{
						army->isAttacked = true;
						army->LostBlood(GetDame());
						this->isAttack = false;
					}
					else
					{
						if (isBurning)
						{
							army->isAttacked = true;
							army->LostBlood(GetDame());
							this->isAttack = false;
						}
					}
				}
			}
			else if (dynamic_cast<CZombie*>(coObjects->at(i))) {
				CZombie* zombie = dynamic_cast<CZombie*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				zombie->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					if (this->state != WEAPON_STATE_FIRE)
					{
						zombie->isAttacked = true;
						zombie->LostBlood(GetDame());
						this->isAttack = false;
					}
					else
					{
						if (isBurning)
						{
							zombie->isAttacked = true;
							zombie->LostBlood(GetDame());
							this->isAttack = false;
						}
					}
				}
			}
			else if (dynamic_cast<CBat*>(coObjects->at(i))) {
				CBat* bat = dynamic_cast<CBat*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				bat->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					if (this->state != WEAPON_STATE_FIRE)
					{
						bat->isAttacked = true;
						bat->LostBlood(GetDame());
						isAttack = false;
					}
					else
					{
						if (isBurning)
						{
							bat->isAttacked = true;
							bat->LostBlood(GetDame());
							isAttack = false;
						}
					}
				}
			}
			else if (dynamic_cast<CFlea*>(coObjects->at(i))) {
				CFlea* flea = dynamic_cast<CFlea*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				flea->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					if (this->state != WEAPON_STATE_FIRE)
					{
						flea->isAttacked = true;
						flea->LostBlood(GetDame());
						isAttack = false;
					}
					else
					{
						if (isBurning)
						{
							flea->isAttacked = true;
							flea->LostBlood(GetDame());
							isAttack = false;
						}
					}
				}
			}
			else if (dynamic_cast<CBird*>(coObjects->at(i))) {
				CBird* bird = dynamic_cast<CBird*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				bird->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					if (this->state != WEAPON_STATE_FIRE)
					{
						bird->isAttacked = true;
						bird->LostBlood(GetDame());
						isAttack = false;
					}
					else
					{
						if (isBurning)
						{
							bird->isAttacked = true;
							bird->LostBlood(GetDame());
							isAttack = false;
						}
					}
				}
			}
			else if (dynamic_cast<CToad*>(coObjects->at(i))) {
				CToad* toad = dynamic_cast<CToad*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				toad->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					if (this->state != WEAPON_STATE_FIRE)
					{
						toad->isAttacked = true;
						toad->LostBlood(GetDame());
						isAttack = false;
					}
					else
					{
						if (isBurning)
						{
							toad->isAttacked = true;
							toad->LostBlood(GetDame());
							isAttack = false;
						}
					}
				}
			}
			else if (dynamic_cast<CSkeleton*>(coObjects->at(i))) {
				CSkeleton* skeleton = dynamic_cast<CSkeleton*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				skeleton->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					if (this->state != WEAPON_STATE_FIRE)
					{
						skeleton->isAttacked = true;
						skeleton->LostBlood(GetDame());
						isAttack = false;
					}
					else
					{
						if (isBurning)
						{
							skeleton->isAttacked = true;
							skeleton->LostBlood(GetDame());
							isAttack = false;
						}
					}
				}
			}
			else if (dynamic_cast<CBoss*>(coObjects->at(i))) {
				CBoss* boss = dynamic_cast<CBoss*>(coObjects->at(i));

				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				boss->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					if (this->state != WEAPON_STATE_FIRE)
					{
						boss->isAttacked = true;
						boss->LostBlood(GetDame());
						isAttack = false;
					}
					else
					{
						if (isBurning)
						{
							boss->isAttacked = true;
							boss->LostBlood(GetDame());
							isAttack = false;
						}
					}
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
		(nx > 0) ? vx = KNIFE_SPEED : vx = -KNIFE_SPEED;
		vy = 0;
		this->dame = 1;
		break;
	case WEAPON_STATE_AXE:
		(nx > 0) ? vx = AXE_SPEED : vx = -AXE_SPEED;
		vy = -AXE_SPEED_Y;
		this->dame = 2;
		break;
	case WEAPON_STATE_BOMERANG:
		(nx > 0) ? vx = BOMERANG_SPEED : vx = -BOMERANG_SPEED;
		vy = 0;
		this->dame = 2;
		break;
	case WEAPON_STATE_FIRE:
		if (isBurning == true)
			vx = 0;
		else
			(nx > 0) ? vx = BOMERANG_SPEED : vx = -BOMERANG_SPEED;
		(isBurning) ? vy = 0 : vy = -FIRE_SPEED_Y;
		this->dame = 1;
		break;
	default:
		break;
	}
}
