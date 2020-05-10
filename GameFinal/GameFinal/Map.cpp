#include "Map.h"

CMap::CMap()
{

}

void CMap::Add(int col, int row, int idTile)
{
	tileMap[col][row] = idTile + TILEMAP_ID_START;
}

void CMap::Render(int xCam, int yCam)
{
	int startCol = (int)xCam / 32;
	int endCol = startCol + CGame::GetInstance()->GetScreenWidth() / 32;

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

	//int startCol = 0;
	////int endCol = startCol + 480 / 32;
	//int endCol = this->width / 32;
	//int numOfRow = this->rowNumber;
	//for (int i = 0; i < numOfRow; i++)
	//{
	//	for (int j = startCol; j < endCol; j++)
	//	{
	//		float x = 32 * (j - startCol);
	//		float y = 32 * i;

	//		CSprites::GetInstance()->Get(tileMap[j][i])->Draw(x, y);
	//	}
	//}
}
