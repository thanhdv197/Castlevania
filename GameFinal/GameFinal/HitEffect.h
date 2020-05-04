#pragma once
#include "GameObject.h"

class CHitEffect : public CGameObject
{
	DWORD timeDisplay;
public:
	bool isEnable;

	CHitEffect() { 
		isEnable = true;
		timeDisplay = 0;
		SetAnimationSet(CAnimationSets::GetInstance()->Get(46));
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

