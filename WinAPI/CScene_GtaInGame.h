#pragma once
#include "CScene.h"
class CScene_GtaInGame : public CScene
{
public:
	virtual void PrevUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

	virtual void Start() override;
	virtual void End() override;

public:
	CScene_GtaInGame();
	virtual ~CScene_GtaInGame() override;
};