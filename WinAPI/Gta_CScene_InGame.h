#pragma once
#include "CScene.h"
class Gta_CScene_InGame : public CScene
{
public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void Start() override;
	virtual void End() override;
private:
	Gta_CScene_InGame();
	virtual ~Gta_CScene_InGame() override;
};

