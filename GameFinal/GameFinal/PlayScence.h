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
#include "Item.h"
#include "Map.h"
#include "Torch.h"


class CPlayScene: public CScene
{
protected: 
	CSimon *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;

	CMap *map;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
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

