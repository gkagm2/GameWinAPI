#pragma once
#include "CScene.h"
class CTileMap;
class CScene_Tool : public CScene
{
	HMENU m_hMenu; // 메뉴 핸들

	CTileMap* m_pTileMap;

public:
	virtual void Update() override;
	virtual void Start() override;
	virtual void End() override;

public:
	
	void MouseClick();
	CTileMap* GetTileMap() { return m_pTileMap; }
	void SaveTile(wstring _strPath);
	void LoadTile(wstring _strPath);

public:
	CScene_Tool();
	virtual ~CScene_Tool() override;
};