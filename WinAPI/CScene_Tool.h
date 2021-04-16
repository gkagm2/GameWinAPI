#pragma once
#include "CScene.h"
class CTileMap;
class CTexture;
class CScene_Tool : public CScene
{
	HMENU m_hMenu; // 메뉴 핸들

public:
	virtual void Update() override;
	virtual void Start() override;
	virtual void End() override;

public:

	void SaveTile(wstring _strPath);
	void SaveItem(wstring _strPath);

public:
	CScene_Tool();
	virtual ~CScene_Tool() override;
};