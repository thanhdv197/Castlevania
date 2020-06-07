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
	isHurt = false;

	timeLevelUp = 0;
	timeHurt = 0;

	usingWhip = false;
	isFlyingWeapon = false;

	isStairUp = false;
	isStairDown = false;
	isGoUp = false;
	isGoDown = false;

	isFinish = false;

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
	if (isGoUp == false && isGoDown == false && isAttack == false)
	{
		vy += SIMON_GRAVITY * dt;
	}
	else
	{
		/*x += dx;
		y += dy;*/

		// finish stair state
		if (abs(this->x - this->xStair) > 80)
		{
			isGoUp = false;
			isGoDown = false;
			isStairUp = false;
			isStairDown = false;
		}
	}

	// set hurt state
	if (isHurt)
	{
		timeHurt += dt;

		// set position when hurt
		/*if (nx > 0)
			x -= 1;
		else x += 1;*/

		if (timeHurt > 300)
		{
			SetState(SIMON_STATE_SIT);
			if (isSit)
			{
				if (timeHurt > 600)
				{
					isHurt = false;
					timeHurt = 0;
				}
			}
		}
	}

	if (this->y > CGame::GetInstance()->GetScreenHeight())
		SetState(SIMON_STATE_DIE);

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
					x += dx;
				}
				else
				{
					CollisionItem(torch->GetItem());
					torch->isDisplay = false;
				}
			}
			else if (dynamic_cast<CCandle *>(e->obj))
			{
				CCandle *candle = dynamic_cast<CCandle *>(e->obj);

				if (candle->GetState() == STATE_CANDLE)
				{
					x += dx;
				}
				else
				{
					CollisionItem(candle->GetItem());
					candle->isDisplay = false;
				}
			}
			else if (dynamic_cast<CArmy *>(e->obj))
			{
				CArmy *army = dynamic_cast<CArmy *>(e->obj);

				if (army->GetState() == STATE_ARMY)
				{
					SetState(SIMON_STATE_HURT);
				}
				else
				{
					CollisionItem(army->GetItem());
					army->isDisplay = false;
				}
			}
			else if (dynamic_cast<CZombie *>(e->obj))
			{
				CZombie *zombie = dynamic_cast<CZombie *>(e->obj);

				if (zombie->GetState() == STATE_ZOMBIE)
				{
					SetState(SIMON_STATE_HURT);
				}
				else
				{
					CollisionItem(zombie->GetItem());
					zombie->isDisplay = false;
				}
			}
			else if (dynamic_cast<CBat *>(e->obj))
			{
				CBat *bat = dynamic_cast<CBat *>(e->obj);

				if (bat->GetState() != STATE_ITEM)
				{
					SetState(SIMON_STATE_HURT);
				}
				else
				{
					CollisionItem(bat->GetItem());
					bat->isDisplay = false;
				}
			}
			else if (dynamic_cast<CFlea *>(e->obj))
			{
				CFlea *flea = dynamic_cast<CFlea *>(e->obj);

				if (flea->GetState() != STATE_ITEM)
				{
					SetState(SIMON_STATE_HURT);
				}
				else
				{
					CollisionItem(flea->GetItem());
					flea->isDisplay = false;
				}
			}
			else if (dynamic_cast<CBird *>(e->obj))
			{
				CBird *bird = dynamic_cast<CBird *>(e->obj);

				if (bird->GetState() != STATE_ITEM)
				{
					SetState(SIMON_STATE_HURT);
				}
				else
				{
					CollisionItem(bird->GetItem());
					bird->isDisplay = false;
				}
			}
			else if (dynamic_cast<CToad *>(e->obj))
			{
				CToad *toad = dynamic_cast<CToad *>(e->obj);

				if (toad->GetState() != STATE_ITEM)
				{
					SetState(SIMON_STATE_HURT);
				}
				else
				{
					CollisionItem(toad->GetItem());
					toad->isDisplay = false;
				}
			}
			else if (dynamic_cast<CSkeleton *>(e->obj))
			{
				CSkeleton *skeleton = dynamic_cast<CSkeleton *>(e->obj);

				if (skeleton->GetState() != STATE_ITEM)
				{
					SetState(SIMON_STATE_HURT);
				}
				else
				{
					CollisionItem(skeleton->GetItem());
					skeleton->isDisplay = false;
				}
			}
			else if (dynamic_cast<CBone *>(e->obj))
			{
				CBone *bone = dynamic_cast<CBone *>(e->obj);

				if (bone->GetState() == STATE_THROW)
				{
					SetState(SIMON_STATE_HURT);
				}
			}
			else if (dynamic_cast<CBoss *>(e->obj))
			{
				CBoss *boss = dynamic_cast<CBoss *>(e->obj);

				if (boss->GetState() != STATE_GLOBULAR)
				{
					SetState(SIMON_STATE_HURT);
				}
				else
				{
					boss->isDisplay = false;
					isFinish = true;
				}
			}
			else if (dynamic_cast<CBottomStair *>(e->obj))
			{
				CBottomStair *bottomStair = dynamic_cast<CBottomStair *>(e->obj);
				this->isStairUp = true;
				this->isStairDown = false;

				x += dx;

				this->stairDirection = bottomStair->GetDirection();
				bottomStair->GetPosition(xStair, yStair);
				
			}
			else if (dynamic_cast<CTopStair *>(e->obj))
			{
				CTopStair *topStair = dynamic_cast<CTopStair *>(e->obj);
				this->isStairDown = true;
				this->isStairUp = false;

				x += dx;
				/*if ( ny<0 )
					y += dy;*/

				this->stairDirection = topStair->GetDirection();
				topStair->GetPosition(xStair, yStair);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	// check state of object (add scores for player)
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBottomStair*>(coObjects->at(i))) {
			CBottomStair* bottomStair = dynamic_cast<CBottomStair*>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			bottomStair->GetBoundingBox(l2, t2, r2, b2);

			if (CGame::GetInstance()->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
			{
				this->isStairUp = true;
				this->isStairDown = false;
			}
		}
		else if (dynamic_cast<CTopStair*>(coObjects->at(i))) {
			CTopStair* topStair = dynamic_cast<CTopStair*>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			topStair->GetBoundingBox(l2, t2, r2, b2);

			if (CGame::GetInstance()->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
			{
				this->isStairDown = true;
				this->isStairUp = false;
			}
		}
		else if (dynamic_cast<CTorch *>(coObjects->at(i)))
		{
			CTorch *torch = dynamic_cast<CTorch *>(coObjects->at(i));

			if (torch->GetBlood() < 1 && torch->GetBlood() > -10)
			{
				torch->SetBlood(1000);
				AddScores(1000);
			}
		}
		else if (dynamic_cast<CCandle*>(coObjects->at(i))) {
			CCandle* candle = dynamic_cast<CCandle*>(coObjects->at(i));

			if (candle->GetBlood() < 1 && candle->GetBlood() > -10)
			{
				candle->SetBlood(1000);
				AddScores(1000);
			}
		}
		else if (dynamic_cast<CArmy*>(coObjects->at(i))) {
			CArmy* army = dynamic_cast<CArmy*>(coObjects->at(i));

			if (army->GetBlood() < 1 && army->GetBlood() > -10)
			{
				army->LostBlood(1000);
				AddScores(1000);
			}
		}
		else if (dynamic_cast<CZombie*>(coObjects->at(i))) {
			CZombie* zombie = dynamic_cast<CZombie*>(coObjects->at(i));

			if (zombie->GetBlood() < 1 && zombie->GetBlood() > -10)
			{
				zombie->LostBlood(1000);
				AddScores(1000);
			}
		}
		else if (dynamic_cast<CBat*>(coObjects->at(i))) {
			CBat* bat = dynamic_cast<CBat*>(coObjects->at(i));

			if (bat->GetBlood() < 1 && bat->GetBlood() > -10)
			{
				bat->LostBlood(1000);
				AddScores(1000);
			}
		}
		else if (dynamic_cast<CFlea*>(coObjects->at(i))) {
			CFlea* flea = dynamic_cast<CFlea*>(coObjects->at(i));

			if (flea->GetBlood() < 1 && flea->GetBlood() > -10)
			{
				flea->LostBlood(1000);
				AddScores(1000);
			}
		}
		else if (dynamic_cast<CBird*>(coObjects->at(i))) {
			CBird* bird = dynamic_cast<CBird*>(coObjects->at(i));

			if (bird->GetBlood() < 1 && bird->GetBlood() > -10)
			{
				bird->LostBlood(1000);
				AddScores(1000);
			}
		}
		else if (dynamic_cast<CToad*>(coObjects->at(i))) {
			CToad* toad = dynamic_cast<CToad*>(coObjects->at(i));

			if (toad->GetBlood() < 1 && toad->GetBlood() > -10)
			{
				toad->LostBlood(1000);
				AddScores(1000);
			}
		}
		else if (dynamic_cast<CSkeleton*>(coObjects->at(i))) {
			CSkeleton* skeleton = dynamic_cast<CSkeleton*>(coObjects->at(i));

			if (skeleton->GetBlood() < 1 && skeleton->GetBlood() > -10)
			{
				skeleton->LostBlood(1000);
				AddScores(1000);
			}
		}
		else if (dynamic_cast<CBoss*>(coObjects->at(i)))
		{
			CBoss* boss = dynamic_cast<CBoss*>(coObjects->at(i));

			if (boss->GetBlood() < 1 && boss->GetBlood() > -10)
			{
				boss->LostBlood(1000);
				AddScores(10000);
			}
		}
	}

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
	else if (state == SIMON_STATE_HURT)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_HURT_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_HURT_LEFT;
		}
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
	else if (state == SIMON_STATE_GO_UP && isGoUp == true)
	{
		if (nx > 0)
		{
			if (isAttack)
				ani = SIMON_ANI_ATTACK_UP_RIGHT;
			else
				ani = SIMON_ANI_GO_UP_RIGHT;
		}
		else
		{
			if (isAttack)
				ani = SIMON_ANI_ATTACK_UP_LEFT;
			else
				ani = SIMON_ANI_GO_UP_LEFT;
		}
	}
	else if (state == SIMON_STATE_GO_DOWN && isGoDown == true)
	{
		if (nx > 0)
		{
			if (isAttack)
				ani = SIMON_ANI_ATTACK_DOWN_RIGHT;
			else
				ani = SIMON_ANI_GO_DOWN_RIGHT;
		}
		else
		{
			if (isAttack)
				ani = SIMON_ANI_ATTACK_DOWN_LEFT;
			else
				ani = SIMON_ANI_GO_DOWN_LEFT;
		}
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
			else if (isHurt)
			{
				if (nx > 0)
					ani = SIMON_ANI_HURT_RIGHT;
				else ani = SIMON_ANI_HURT_LEFT;
			}
			else
			{
				if (isGoUp == true && isGoDown == false)
				{
					if (nx > 0)
						ani = SIMON_ANI_IDLE_GO_UP_RIGHT;
					else ani = SIMON_ANI_IDLE_GO_UP_LEFT;
				}
				else if (isGoUp == false && isGoDown == true)
				{
					if (nx > 0)
						ani = SIMON_ANI_IDLE_GO_DOWN_RIGHT;
					else ani = SIMON_ANI_IDLE_GO_DOWN_LEFT;
				}
				else
				{
					if (nx > 0)
						ani = SIMON_ANI_IDLE_RIGHT;
					else ani = SIMON_ANI_IDLE_LEFT;
				}
				
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
		isGoUp = false;
		isGoDown = false;
		isStairUp = false;
		isStairDown = false;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		isGoUp = false;
		isGoDown = false;
		isStairUp = false;
		isStairDown = false;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		isJump = true;
		vy = -SIMON_JUMP_SPEED_Y;
		isGoUp = false;
		isGoDown = false;
		isStairUp = false;
		isStairDown = false;
		break;
	case SIMON_STATE_IDLE:
		isSit = false;
		isJump = false;
		vx = 0;
		if (isGoUp == true || isGoDown == true)
		{
			vy = 0;
		}
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		if (this->alive > 0)
		{
			this->alive--;
			Reset();
		}
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
		vy = 0.05f;
		break;
	case SIMON_STATE_SIT:
		isSit = true;
		break;
	case SIMON_STATE_GO_UP:
		if (this->isStairUp)
		{
			this->isGoUp = true;
			this->isGoDown = false;

			nx = stairDirection;
			vy = -0.01f;
			if (nx > 0)
				vx = 0.01f;
			else vx = -0.01f;
		}
		break;
	case SIMON_STATE_GO_DOWN:
		if (this->isStairDown)
		{
			this->isGoUp = false;
			this->isGoDown = true;

			nx = stairDirection;
			vy = 0.01f;
			if (nx > 0)
				vx = 0.01f;
			else vx = -0.01f;
		}
		break;
	case SIMON_STATE_HURT:
		isHurt = true;
		LostBlood(1);
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

void CSimon::TotalScores(int _heart, DWORD _time)
{
	if (_heart > 0)
	{
		this->heart--;
		this->score += 100;
	}
	
	if (_time > 0)
	{
		this->score += 10;
	}
}


