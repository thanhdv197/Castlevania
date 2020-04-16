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

	isJump = false;
	isAttack = false;
	isSit = false;
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	if (isAttack && animation_set->at(ani)->GetCurrentFrame() >= 2)
	{
		isAttack = false;
	}

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
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
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
		ani = SIMON_ANI_SIT_RIGHT;
		isSit = false;
	}
	else if (state == SIMON_STATE_JUMP)
	{
		if (isJump)
		{
			ani = SIMON_ANI_JUMP_RIGHT;
		}
		else return;
	}
	else
	{
		if (vx == 0)
		{
			if (isAttack)
			{
				ani = SIMON_ANI_ATTACK_RIGHT;
			}
			else
			{
				ani = SIMON_ANI_IDLE_RIGHT;
			}

		}
		else if (vx > 0)
			ani = SIMON_ANI_WALKING_RIGHT;
		else ani = SIMON_ANI_WALKING_RIGHT;
	}

	ani += (nx > 0) ? 0 : 1;

	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(x, y, alpha);

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
		break;
	case SIMON_STATE_ATTACK:
		isAttack = true;
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

	if (state == SIMON_STATE_SIT || state == SIMON_STATE_JUMP)
	{
		right = x + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT - 7;
	}
	else
	{
		right = x + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;
	}

}

void CSimon::Reset()
{
	SetState(SIMON_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

