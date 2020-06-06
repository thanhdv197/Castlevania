#include "Scores.h"

CScores::CScores()
{
	// create font
	D3DXCreateFont(
		CGame::GetInstance()->GetDirect3DDevice(),
		10,
		10,
		FW_NORMAL,
		1,
		false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		L"Arial",
		&font
	);
}

void CScores::Update(int _scores, int _stage, int _heart, int _alive, int _blood, int _stateWeapon, int _bloodBoss, DWORD dt)
{
	this->scores = _scores;
	this->stage = _stage;
	this->heart = _heart;
	this->alive = _alive;
	this->bloodSimon = _blood;
	this->stateWeapon = _stateWeapon;
	this->bloodBoss = _bloodBoss;

	timeLimit += dt;
	if (timeLimit > 1000)
	{
		time -= 1;

		timeLimit = 0;
	}
}

void CScores::Render()
{
	// get camera
	int camX = CGame::GetInstance()->GetCamPosX();

	// draw scores bar
	spriteScores = CSprites::GetInstance()->Get(20099);
	spriteScores->Draw(camX, 0);

	// draw text
	if (font)
	{
		// draw scores
		SetRect(&rectScores, 51, 6, 121, 16);
		string strScores = "000000";
		// check length of scores == 6
		if (this->scores != NULL)
		{
			strScores = to_string(this->scores);
			CheckLength(strScores, 6);
		}
		Draw(rectScores, strScores);

		// draw time
		SetRect(&rectTime, 164, 6, 214, 16);
		string strTime = "0000";
		// check length of time
		if (this->time != NULL)
		{
			strTime = to_string(this->time);
			CheckLength(strTime, 4);
		}
		Draw(rectTime, strTime);

		// draw stage
		SetRect(&rectStage, 256, 6, 280, 16);
		string strStage = "00";
		if (this->stage != NULL)
		{
			strStage = to_string(this->stage);
			CheckLength(strStage, 2);
		}
		Draw(rectStage, strStage);

		// draw heart
		SetRect(&rectHeart, 211, 16, 250, 24);
		string strHeart = "00";
		if (this->heart != NULL)
		{
			strHeart = to_string(this->heart);
			CheckLength(strHeart, 2);
		}
		Draw(rectHeart, strHeart);

		// draw alive
		SetRect(&rectAlive, 211, 24, 235, 32);
		string strAlive = "00";
		if (this->alive != NULL)
		{
			strAlive = to_string(this->alive);
			CheckLength(strAlive, 2);
		}
		Draw(rectAlive, strAlive);
	}

	// draw blood lost
	spriteBloodLost = CSprites::GetInstance()->Get(20096);
	for (int i = 0; i < 16; i++)
	{
		int x = camX + 50 + 6 * i;
		int y = 16;

		spriteBloodLost->Draw(x, y);

		spriteBloodLost->Draw(x, y + 8);
	}

	// draw blood simon
	spriteBloodSimon = CSprites::GetInstance()->Get(20098);
	for (int i = 0; i < bloodSimon; i++)
	{
		int x = camX + 50 + 6 * i;
		int y = 16;

		spriteBloodSimon->Draw(x, y);
	}

	// draw blood boss
	spriteBloodBoss = CSprites::GetInstance()->Get(20097);
	for (int i = 0; i < bloodBoss; i++)
	{
		int x = camX + 50 + 6 * i;
		int y = 24;

		spriteBloodBoss->Draw(x, y);
	}

	// draw weapon
	if (stateWeapon != WEAPON_STATE_NONE)
	{
		if (stateWeapon == WEAPON_STATE_KNIFE)
			CSprites::GetInstance()->Get(30007)->Draw(camX + 163, 19);
		else if(stateWeapon == WEAPON_STATE_AXE)
			CSprites::GetInstance()->Get(30017)->Draw(camX + 163, 19);
		else if(stateWeapon == WEAPON_STATE_BOMERANG)
			CSprites::GetInstance()->Get(30011)->Draw(camX + 163, 19);
	}

}

void CScores::CheckLength(string &text, int max)
{
	string zero = "0";
	if (text.length() < max)
	{
		do
		{
			text = zero + text;
		} while (text.length() < max);
	}
}

void CScores::Draw(RECT rect, string text)
{
	font->DrawTextA(
		CGame::GetInstance()->GetSpriteHandler(),
		text.c_str(),
		-1,
		&rect,
		DT_LEFT,
		0xFFFFFFFF
	);
}

CScores::~CScores()
{
	if (font)
	{
		font->Release();
		font = NULL;
	}
}