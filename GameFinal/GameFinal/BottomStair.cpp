#include "BottomStair.h"

CBottomStair::CBottomStair(float _x, float _y, int _nx)
{
	x = _x;
	y = _y;

	nx = _nx;
}

void CBottomStair::Render()
{
	RenderBoundingBox();
}

void CBottomStair::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + 32;
	b = y + 32;
}