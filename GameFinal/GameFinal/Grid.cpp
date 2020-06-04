#include "Grid.h"

CGrid::CGrid()
{
	if (792 % CELL_WIDTH == 0)
		numCol = 792 / CELL_WIDTH;
	else numCol = 792 / CELL_WIDTH + 1;

	if (192 % CELL_HEIGHT == 0)
		numRow = 192 / CELL_HEIGHT;
	else numRow = 192 / CELL_HEIGHT + 1;

	cells = new CCell*[numCol];
	for (int i = 0; i < numCol; i++)
		cells[i] = new CCell[numRow];

	objects.clear();
}

void CGrid::Add(LPGAMEOBJECT object, float x, float y)
{
	int startX = x / CELL_WIDTH;
	int startY = y / CELL_HEIGHT;
	int endX = (x + object->GetWidth()) / CELL_WIDTH;
	int endY = (y + object->GetHeight()) / CELL_HEIGHT;

	for (int i = startX; i <= endX; i++)
	{
		/*for (int j = startY; j <= endY; j++)
		{
			cells[i][j].Add(object);
		}*/
		cells[i][startY].Add(object);
	}
	//cells[startX][startY].Add(object);
}

vector<LPGAMEOBJECT> CGrid::GetList()
{
	CGame * game = CGame::GetInstance();

	float x, y;
	x = game->GetCamPosX();
	y = game->GetCamPosY();

	int start = x / CELL_WIDTH;
	int end = (x + game->GetScreenWidth()) / CELL_WIDTH;

	objects.clear();

	for (int i = start; i <= end; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (cells[i][j].GetObjects().size() > 0)
			{
				for (int k = 0; k < cells[i][j].GetObjects().size(); k++)
				{
					objects.push_back(cells[i][j].GetObjects().at(k));
				}
			}
		}
	}

	return objects;
}

void CGrid::Unload()
{
	for (int i = 0; i < numCol; i++)
	{
		for (int j = 0; j < numRow; j++)
		{
			cells[i][j].Unload();
		}
	}

	objects.clear();
}
