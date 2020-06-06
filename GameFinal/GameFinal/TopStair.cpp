#include "TopStair.h"

CTopStair::CTopStair(float _x, float _y, int _nx)
{
	x = _x;
	y = _y;

	nx = _nx;

	width = TOP_STAIR_WIDTH;
	height = TOP_STAIR_HEIGHT;
}

void CTopStair::Render()
{
	RenderBoundingBox();
}

void CTopStair::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}