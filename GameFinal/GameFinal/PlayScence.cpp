#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	
	scores = new CScores();

	map = new CMap();

	grid = new CGrid();
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP	7

#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_BOTTOM_STAIR	1
#define OBJECT_TYPE_TOP_STAIR	2
#define OBJECT_TYPE_FLYBRICK	3
#define OBJECT_TYPE_TORCH	4
#define OBJECT_TYPE_CANDLE	6

#define OBJECT_TYPE_ENEMY_ARMY	10
#define OBJECT_TYPE_ENEMY_ZOMBIE	11
#define OBJECT_TYPE_ENEMY_BAT	12
#define OBJECT_TYPE_ENEMY_FLEA	13
#define OBJECT_TYPE_ENEMY_TOAD	14
#define OBJECT_TYPE_ENEMY_BIRD	15
#define OBJECT_TYPE_ENEMY_SKELETON	16
#define OBJECT_TYPE_ENEMY_BOSS	17

#define OBJECT_TYPE_PORTAL	50

#define OBJECT_TYPE_BRICKS	99

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		//DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
		if (player!=NULL) 
		{
			//DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		//obj = new CSimon(x, y);
		//player = (CSimon*)obj;
		player = new CSimon(x, y);
		player->SetPosition(x, y);
		player->SetAnimationSet(animation_sets->Get(ani_set_id));

		//DebugOut(L"[INFO] Player object created!\n");
		//break;
		return;
	case OBJECT_TYPE_BOTTOM_STAIR:
		{
			int direction = atoi(tokens[4].c_str());
			obj = new CBottomStair(x, y, direction); 
			break;
		}
	case OBJECT_TYPE_TOP_STAIR:
		{
			int direction = atoi(tokens[4].c_str());
			obj = new CTopStair(x, y, direction);
			break;
		}
	case OBJECT_TYPE_FLYBRICK:
		{
			obj = new CFlyBrick();
			break;
		}
	case OBJECT_TYPE_TORCH:
		{
			int item = atoi(tokens[4].c_str());
			obj = new CTorch(item);
			break;
		}
	case OBJECT_TYPE_CANDLE:
		{
			int item = atoi(tokens[4].c_str());
			obj = new CCandle(item);
			break;
		}
	case OBJECT_TYPE_ENEMY_ARMY:
		{
			int item = atoi(tokens[4].c_str());
			obj = new CArmy(item);
			break;
		}
	case OBJECT_TYPE_ENEMY_ZOMBIE:
		{
			int item = atoi(tokens[4].c_str());
			obj = new CZombie(item);
			break;
		}
	case OBJECT_TYPE_ENEMY_BAT:
		{
			int item = atoi(tokens[4].c_str());
			obj = new CBat(item);
			break;
		}
	case OBJECT_TYPE_ENEMY_FLEA:
		{
			int item = atoi(tokens[4].c_str());
			obj = new CFlea(item);
			break;
		}
	case OBJECT_TYPE_ENEMY_TOAD:
		{
			int item = atoi(tokens[4].c_str());
			obj = new CToad(item);
			break;
		}
	case OBJECT_TYPE_ENEMY_BIRD:
		{
			int item = atoi(tokens[4].c_str());
			obj = new CBird(item);
			break;
		}
	case OBJECT_TYPE_ENEMY_SKELETON:
		{
			int item = atoi(tokens[4].c_str());
			obj = new CSkeleton(item);
			break;
		}
	case OBJECT_TYPE_ENEMY_BOSS:
		{
			obj = new CBoss();
			break;
		}
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;
	case OBJECT_TYPE_BRICKS:
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			obj = new CBricks(x, y, r, b);
		}
	break;
	default:
		//DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);

	// add objects to grid
	grid->Add(obj, x, y);
}

void CPlayScene::_ParseSection_MAP(string line)
{
	//map = CMap::GetInstance();

	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines
	else if (tokens.size() == 3)
	{
		// set width and height map
		map->SetWidth(atoi(tokens[0].c_str()));
		map->SetHeight(atoi(tokens[1].c_str()));
		// set map name
		mapName = atoi(tokens[2].c_str());

		int colNumber = map->GetWidth() / 32;
		int rowNumber = map->GetHeight() / 32;

		map->SetColNumber(colNumber);
		map->SetRowNumber(rowNumber);
	}
	else
	{
		// set tile map
		for (int i = 0; i < tokens.size(); i++)
		{
			int ID = atoi(tokens[i].c_str());
			map->Add(i, map->GetRowCurrent(), ID);
		}
		map->NextRow();
	}
}

void CPlayScene::Load(int _level, int _blood, int _heart, int _alive, int _scores, int _weapon)
{
	//DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	if (player != NULL)
	{
		player->SetWhip(_level);
		player->SetBlood(_blood);
		player->SetHeart(_heart);
		player->SetAlive(_alive);
		player->SetScores(_scores);
		player->SetWeapon(_weapon);
	}

	//DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// create blood for boss
	int bloodBoss = 16;

	// get objects from grid 
	objects = grid->GetList();

	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		// get position simon to change state of enemy
		float xSimon, ySimon;
		player->GetPosition(xSimon, ySimon);
		objects[i]->SetPositionSimon(xSimon, ySimon);

		// set blood of boss
		objects[i]->Update(dt, &coObjects);
		if (objects[i]->IsBoss())
		{
			bloodBoss = objects[i]->GetBloodBoss();
		}
	}

	// time scores
	DWORD time = dt;

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	// update simon
	if (player == NULL) return; 
	else
	{
		player->Update(dt, &coObjects);
		if (player->isFinish)
		{
			(scores->GetTime() > 0) ? time = 1000 : time = 0;
			
			player->TotalScores(player->GetHeart(), time);
		}	
	}

	CGame *game = CGame::GetInstance();
	
	// update Scores bar
	if (player != NULL)
		scores->Update(player->GetScores(), mapName, player->GetHeart(), player->GetAlive(), player->GetBlood(), player->GetWeapon(), bloodBoss, time);

	// change scene
	if (game->GetIsNextMap() == true)
	{
		// get value of simon
		int level = player->GetLevelWhip();
		int blood = player->GetBlood();
		int heart = player->GetHeart();
		int alive = player->GetAlive();
		int scores = player->GetScores();
		int weapon = player->GetWeapon();

		// switch scene
		game->SwitchScene(game->GetSceneId(), level, blood, heart, alive, scores, weapon);
		game->SetIsNextMap(false);
	}

	// Update camera to follow mario
	float cx = 0, cy = 0;

	if (player != NULL)
	{
		player->GetPosition(cx, cy);
	}

	if (cx < game->GetScreenWidth() / 2)
	{
		cx = 0;
		cy = 0;
	}
	else if (map->GetWidth() - cx < game->GetScreenWidth() / 2)
	{
		cx = map->GetWidth() - game->GetScreenWidth();
	}
	else
	{
		cx -= game->GetScreenWidth() / 2;
		cy -= game->GetScreenHeight() / 2;
	}
	
	//if(cx + game->GetScreenWidth() <= map->GetWidth())
	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
	
	
}

void CPlayScene::Render()
{
	// render map follow camera
	map->Render();

	// render objects
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();

	// render simon
	player->Render();

	// render scores bar
	scores->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	/*for (int i = 0; i < objects.size(); i++)
		delete objects[i];*/

	objects.clear();
	player = NULL;

	// grid unload
	grid->Unload();

	//DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CSimon *simon = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_A: 
		simon->Reset();
		break;
	case DIK_S:
		simon->usingWhip = true;
		simon->SetState(SIMON_STATE_ATTACK);
		break;
	case DIK_D:
		simon->usingWhip = false;
		simon->SetState(SIMON_STATE_ATTACK);
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CSimon *simon = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;

	if (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_L))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT) || game->IsKeyDown(DIK_J))
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_DOWN) || game->IsKeyDown(DIK_K))
	{
		if (simon->isStairDown == true)
			simon->SetState(SIMON_STATE_GO_DOWN);
		else simon->SetState(SIMON_STATE_SIT);
	}
	else if (game->IsKeyDown(DIK_SPACE)) {
		if (game->IsKeyDown(DIK_SPACE)) return;
		else
			simon->SetState(SIMON_STATE_JUMP);
	}
	else if (game->IsKeyDown(DIK_UP) || game->IsKeyDown(DIK_I))
	{
		simon->SetState(SIMON_STATE_GO_UP);
	}
	else
	{
		simon->SetState(SIMON_STATE_IDLE);
	}
		
}