#pragma once
#include "CScene.h"
class CTileMap;
class CScene_Tool : public CScene
{
	HMENU m_hMenu; // �޴� �ڵ�

	CTileMap* m_pTileMap;

public:
	virtual void Update() override;
	virtual void Start() override;
	virtual void End() override;

public:
	
	void MouseClick();
	CTileMap* GetTileMap() { return m_pTileMap; }
	void SaveTile(wstring _strRelativePath);
	void LoadTile(wstring _strRelativePath);

public:
	CScene_Tool();
	virtual ~CScene_Tool() override;
};