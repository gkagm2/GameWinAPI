#pragma once
#include "CScene.h"
class Gta_CScene_Main : public CScene
{
public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void Start() override;
	virtual void End() override;
private:
	Gta_CScene_Main();
	virtual ~Gta_CScene_Main() override;
};

