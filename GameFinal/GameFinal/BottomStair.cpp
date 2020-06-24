#include "BottomStair.h"

CBottomStair::CBottomStair(float _x, float _y, int _nx, int state_stair, int _numStair)
{
	x = _x;
	y = _y;

	nx = _nx;

	numStair = _numStair;

	width = BOTTOM_STAIR_WIDTH;
	height = BOTTOM_STAIR_HEIGHT;

	for (int i = 0; i < numStair; i++)
	{
		this->stair[i] = new CStair(_nx, state_stair);
	}
	
}

void CBottomStair::Render()
{
	RenderBoundingBox();

	if (nx > 0)
	{
		for (int i = 0; i < numStair; i++)
		{
			this->stair[i]->SetPosition(x + 12 + 32 * i, y + 32 - i * 32);
		}
		/*this->stair[0]->SetPosition(x + 12, y + 32);
		this->stair[1]->SetPosition(x + 44, y);*/
	}
	else
	{
		for (int i = 0; i < numStair; i++)
		{
			this->stair[i]->SetPosition(x - 32 * (i + 1), y + 32 - i * 32);
		}
		/*this->stair[0]->SetPosition(x - 32, y + 32);
		this->stair[0]->SetPosition(x - 64, y);*/
	}
	
	for (int i = 0; i < numStair; i++)
	{
		this->stair[i]->Render();
	}
	
}

void CBottomStair::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}