﻿#include "Map.h"

CMap::CMap()
{
	this->width = 0;
	this->height = 0;

	this->rowCurrent = 0;

	this->colNumber = 0;
	this->rowNumber = 0;
}

void CMap::Add(int col, int row, int idTile)
{
	tileMap[col][row] = idTile + TILEMAP_ID_START;
}

void CMap::Render()
{
	CGame * game = CGame::GetInstance();

	int xCam = game->GetCamPosX();
	int yCam = game->GetCamPosY();

	int startCol = (int)xCam / 32;
	int endCol = this->width / 32;
	if((xCam + game->GetScreenWidth()) < this->width)
		endCol = startCol + game->GetScreenWidth() / 32;

	// xử lí trạng thái thiếu map khi simon di chuyển
	if (endCol < this->width / 32 - 1)
	{
		endCol += 1;
	}

	int numOfRow = this->rowNumber;

	for (int i = 0; i < numOfRow; i++)
	{
		for (int j = startCol; j <= endCol; j++)
		{
			float x = 32 * (j - startCol) + xCam - (int)xCam % 32;
			float y = 32 * i + 20;	// edit to fit scores bar

			if (tileMap[j][i] != NULL)
			{
				CSprites::GetInstance()->Get(tileMap[j][i])->Draw(x, y);
			}
			
		}
	}
}
