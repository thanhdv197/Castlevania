#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Simon.h"
#include "Bricks.h"
#include "Map.h"
#include "Torch.h"
#include "Candle.h"
#include "FlyBrick.h"
#include "Scores.h"
#include "Army.h"
#include "Zombie.h"
#include "Bat.h"
#include "Flea.h"
#include "Bird.h"
#include "Toad.h"
#include "Skeleton.h"
#include "Boss.h"
#include "Grid.h"
#include "BottomStair.h"
#include "TopStair.h"


class CPlayScene: public CScene
{
protected: 
	CSimon *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;

	CMap *map;
	int mapName = 1;

	CScores * scores;

	CGrid * grid;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load(int _level = 1, int _blood = 16, int _heart = 0, int _alive = 4, int _scores = 0, int _weapon = 0);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CSimon * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

