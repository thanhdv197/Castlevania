#pragma once
#include "Sprites.h"
#include "Game.h"
#include <iostream>  
#include <string>  
#include "Weapon.h"

using namespace std;

class CScores
{
	CSprite * spriteScores;

	ID3DXFont * font = NULL;

	// scores
	RECT rectScores;
	int scores;

	// time
	RECT rectTime;
	int time = 200;
	DWORD timeLimit = 0;

	// stage
	RECT rectStage;
	int stage;

	// heart
	RECT rectHeart;
	int heart;

	// alive
	RECT rectAlive;
	int alive;

	// blood lost
	CSprite * spriteBloodLost;

	// blood of simon
	CSprite * spriteBloodSimon;
	int bloodSimon = 16;

	// blood of boss
	CSprite * spriteBloodBoss;
	int bloodBoss = 16;

	int stateWeapon;
	
public:
	CScores();
	~CScores();

	int GetTime() { return this->time; }

	void Update(int _scores, int _stage, int _heart, int _alive, int _blood, int _stateWeapon, int _bloodBoss, DWORD dt);
	void Render();

	void CheckLength(string &text, int max);

	void Draw(RECT rect, string text);
};

