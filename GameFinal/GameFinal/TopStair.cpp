#include "TopStair.h"

CTopStair::CTopStair(float _x, float _y, int _nx)
{
	x = _x;
	y = _y;

	nx = _nx;
}

void CTopStair::Render()
{
	RenderBoundingBox();
}

void CTopStair::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + 8;
	b = y + 64;
}