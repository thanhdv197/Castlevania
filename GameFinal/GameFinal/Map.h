#pragma once
#include "Sprites.h"
#include "Animations.h"
#include "Game.h"

#define TILEMAP_ID_START	20000;

class CMap
{
	int width = 0;
	int height = 0;

	int colNumber;
	int rowNumber;

	int rowCurrent = 0;

	int tileMap[500][500];

public:
	CMap();
	void Add(int col, int row, int idTile);
	void Render(int xCam, int yCam);

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
