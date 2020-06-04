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
	int i = x / CELL_WIDTH;
	int j = y / CELL_HEIGHT;

	cells[i][j].Add(object);
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
