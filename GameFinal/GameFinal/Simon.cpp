#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Simon.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CSimon::CSimon(float x, float y)
{
	untouchable = 0;
	SetState(SIMON_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	whip = new CWhip();

	this->stateWeapon = WEAPON_STATE_NONE;

	isJump = false;
	isAttack = false;
	isSit = false;
	isLevelUp = false;

	timeLevelUp = 0;

	usingWhip = false;
	isFlyingWeapon = false;

	this->blood = 16;
	this->alive = 4;
	this->heart = 0;
	this->score = 0;
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// level up finish
	if (isLevelUp)
	{
		timeLevelUp += dt;
		if (timeLevelUp > 1000)
			isLevelUp = false;
	}

	// stop with last frame of attack state
	if (isAttack && animation_set->at(ani)->GetCurrentFrame() == 2)
	{
		if (usingWhip)
		{
			if (whip->GetCurrentFrame() == 2)
			{
				usingWhip = false;
			}
			else
			{
				whip->SetCurrentFrame(2);
			}
		}

		isAttack = false;
	}

	// update whip
	whip->Update(dt, coObjects);

	// update weapon
	if (weapon)
	{
		weapon->Update(dt, coObjects);
	}

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

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -SIMON_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							SetState(SIMON_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				//CGame::GetInstance()->SwitchScene(p->GetSceneId());

				CGame::GetInstance()->SetIsNextMap(true);
				CGame::GetInstance()->SetSceneId(p->GetSceneId());
			}
			else if (dynamic_cast<CTorch *>(e->obj)) 
			{
				CTorch *torch = dynamic_cast<CTorch *>(e->obj);

				if (torch->GetState() == STATE_TORCH)
				{

				}
				else
				{
					CollisionItem(torch->GetItem());
					torch->isEnable = false;
				}
			}
			else if (dynamic_cast<CCandle *>(e->obj))
			{
				CCandle *candle = dynamic_cast<CCandle *>(e->obj);

				if (candle->GetState() == STATE_CANDLE)
				{

				}
				else
				{
					CollisionItem(candle->GetItem());
					candle->isEnable = false;
				}
			}
			else if (dynamic_cast<CArmy *>(e->obj))
			{
				CArmy *army = dynamic_cast<CArmy *>(e->obj);

				if (army->GetState() == STATE_ARMY)
				{
					LostBlood(4);
					SetPosition(x + 20, y);
				}
				else
				{
					CollisionItem(army->GetItem());
					army->isEnable = false;
				}
			}
			else if (dynamic_cast<CZombie *>(e->obj))
			{
				CZombie *zombie = dynamic_cast<CZombie *>(e->obj);

				if (zombie->GetState() == STATE_ZOMBIE)
				{
					LostBlood(4);
					SetPosition(x + 20, y);
				}
				else
				{
					CollisionItem(zombie->GetItem());
					zombie->isEnable = false;
				}
			}
			else if (dynamic_cast<CBat *>(e->obj))
			{
				CBat *bat = dynamic_cast<CBat *>(e->obj);

				if (bat->GetState() != STATE_ITEM)
				{
					LostBlood(4);
					SetPosition(x + 20, y);
				}
				else
				{
					CollisionItem(bat->GetItem());
					bat->isEnable = false;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CSimon::Render()
{
	ani = SIMON_ANI_IDLE_RIGHT;

	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else if (state == SIMON_STATE_SIT)
	{
		if (nx > 0)
		{
			if (isAttack)
				ani = SIMON_ANI_SIT_ATTACK_RIGHT;
			else
				ani = SIMON_ANI_SIT_RIGHT;
		}
		else
		{
			if (isAttack)
				ani = SIMON_ANI_SIT_ATTACK_LEFT;
			else
				ani = SIMON_ANI_SIT_LEFT;
		}
		
		isSit = false;
	}
	else if (state == SIMON_STATE_JUMP)
	{
		if (isJump)
		{
			if (nx > 0)
			{
				if (isAttack)
					ani = SIMON_ANI_ATTACK_RIGHT;
				else
					ani = SIMON_ANI_JUMP_RIGHT;
			}
			else
			{
				if (isAttack)
					ani = SIMON_ANI_ATTACK_LEFT;
				else
					ani = SIMON_ANI_JUMP_LEFT;
			}
		}
		else return;
	}
	else
	{
		if (vx == 0)
		{
			if (isAttack)
			{
				if (nx > 0)
					ani = SIMON_ANI_ATTACK_RIGHT;
				else ani = SIMON_ANI_ATTACK_LEFT;
			}
			else if (isLevelUp)
			{
				if (nx > 0)
					ani = SIMON_ANI_LEVELUP_RIGHT;
				else ani = SIMON_ANI_LEVELUP_LEFT;
			}
			else
			{
				if (nx > 0)
					ani = SIMON_ANI_IDLE_RIGHT;
				else ani = SIMON_ANI_IDLE_LEFT;
			}

		}
		else if (vx > 0)
			ani = SIMON_ANI_WALKING_RIGHT;
		else ani = SIMON_ANI_WALKING_LEFT;
	}

	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(x, y, alpha);

	// using whip for attack
	if (isAttack && usingWhip)
	{
		if (state == SIMON_STATE_SIT || state == SIMON_STATE_JUMP)
		{
			whip->SetDirection(nx);
			if (whip->GetDirection() > 0)
			{
				whip->SetPosition(x - 15, y + 10);
			}
			else
			{
				if (whip->GetLevel() > 1)
				{
					whip->SetPosition(x - 40, y + 10);
				}
				else
				{
					whip->SetPosition(x - 20, y + 10);
				}
			}
		}
		else
		{
			whip->SetDirection(nx);
			if (whip->GetDirection() > 0)
			{
				whip->SetPosition(x - 15, y + 2);
			}
			else
			{
				if (whip->GetLevel() > 1)
				{
					whip->SetPosition(x - 40, y + 2);
				}
				else
				{
					whip->SetPosition(x - 20, y + 2);
				}
				
			}
		}

		whip->Render();
		
		whip->isAttack = true;
	}

	// using weapon for attack
	if (weapon)
	{
		if (isAttack && !usingWhip && animation_set->at(ani)->GetCurrentFrame() == 2)
		{
			isFlyingWeapon = true;
		}

		if (isFlyingWeapon)
		{
			weapon->Render();
		}
	}

	RenderBoundingBox();
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		isJump = true;
		vy = -SIMON_JUMP_SPEED_Y;
	case SIMON_STATE_IDLE:
		isSit = false;
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		if(this->alive > 0)
			Reset();
		break;
	case SIMON_STATE_ATTACK:
		isAttack = true;

		if (!usingWhip)
		{
			float xSimon = 0, ySimon = 0;
			//if(CSimon::GetInstance()!=NULL)
			GetPosition(xSimon, ySimon);
			weapon = new CWeapon(xSimon, ySimon, GetDirection(), this->stateWeapon);
			weapon->isAttack = true;
		}
		
		break;
	case SIMON_STATE_SIT:
		isSit = true;
		break;
	}
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;

	if (state == SIMON_STATE_JUMP)
	{
		left = x;
		top = y+7;

		right = x + SIMON_BBOX_WIDTH;
		bottom = y + 23;
	}

}

void CSimon::Reset()
{
	SetState(SIMON_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	this->blood = 16;
}

void CSimon::CollisionItem(int item)
{
	/*
		0: whip item
		1: knife item
		2: axe item
		3: bomerang item
		4: small heart
		5: big heart
		6: fire
	*/
	if (item == 0)
	{
		isLevelUp = true;
		whip->LevelUp();
	}
	else if (item == 1)
	{
		stateWeapon = WEAPON_STATE_KNIFE;
	}
	else if (item == 2)
	{
		stateWeapon = WEAPON_STATE_AXE;
	}
	else if (item == 3)
	{
		stateWeapon = WEAPON_STATE_BOMERANG;
	}
	else if (item == 4)
	{
		heart += 1;
	}
	else if (item == 5)
	{
		heart += 3;
	}
	else
		stateWeapon = WEAPON_STATE_FIRE;
}

void CSimon::LostBlood(int _blood)
{
	if (this->alive > 0)
	{
		if (this->blood > 1)
		{
			this->blood -= _blood;
		}
		else
		{
			this->alive -= 1;
			SetState(SIMON_STATE_DIE);
		}
	}
	else
		SetState(SIMON_STATE_DIE);
	
}


