#pragma once
#include "CScene.h"
class CTileMap;
class CTexture;
class CScene_Tool : public CScene
{
	HMENU m_hMenu; // 메뉴 핸들

	// 클릭한 정보
	bool bIsDrag;
	int iSelectedTileIdx;
	E_TileType eTileType;
	int startDragIdxX;
	int startDragIdxY;
	CTexture* pTileTexture;

public:
	virtual void Update() override;
	virtual void Start() override;
	virtual void End() override;

public:
	
	void MouseClick();
	bool IsTileClicked(const Vector2& _vClickPos);

	void SaveTile(wstring _strPath);


public:
	CScene_Tool();
	virtual ~CScene_Tool() override;
};