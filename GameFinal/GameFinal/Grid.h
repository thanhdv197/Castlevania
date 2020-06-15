#pragma once
#include "GameObject.h"
#include "Game.h"

#define CELL_WIDTH	160
#define CELL_HEIGHT	120

class CCell
{
	vector<LPGAMEOBJECT> objects;

public:
	CCell() { objects.clear(); };
	void Add(LPGAMEOBJECT object) { objects.push_back(object); };
	vector<LPGAMEOBJECT> GetObjects() { return objects; }
	void Unload() { objects.clear(); }
};

class CGrid
{
	int numCol;
	int numRow;
	vector<LPGAMEOBJECT> objects;
	CCell ** cells;

public:
	CGrid();
	void Add(LPGAMEOBJECT object, float x, float y);
	void Add(LPGAMEOBJECT object, int xCell, int yCell);
	vector<LPGAMEOBJECT> GetList();
	void Unload();
};

