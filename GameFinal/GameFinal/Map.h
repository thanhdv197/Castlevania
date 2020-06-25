#pragma once
#include "Sprites.h"
#include "Animations.h"
#include "Game.h"

#define TILEMAP_ID_START	20000
#define ID_NOT_DRAW			20049

class CMap
{
	//static CMap* __instance;

	int width;
	int height;

	int colNumber;
	int rowNumber;

	int rowCurrent;

	int tileMap[500][500];

public:
	//static CMap* GetInstance();
	CMap();
	void Add(int col, int row, int idTile);
	void Render();

	void SetWidth(int width) { this->width = width; }
	int GetWidth() { return this->width; }

	void SetHeight(int height) { this->height = height; }
	int GetHeight() { return this->height; }

	void NextRow() { this->rowCurrent += 1; }
	int GetRowCurrent() { return this->rowCurrent; }

	void SetColNumber(int colNumber) { this->colNumber = colNumber; }
	int GetColNumber() { return this->colNumber; }

	void SetRowNumber(int rowNumber) { this->rowNumber = rowNumber; }
	int GetRowNumber() { return this->rowNumber; }
};

