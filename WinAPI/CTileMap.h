#pragma once
#include "CObject.h"
class CScene;
class CTileMap : public CObject
{
private:
	UINT m_iRow; // 타일 행
	UINT m_iCol; // 타일 열

	UINT m_iTileMapWidth;
	UINT m_iTileMapHeight;
	

public:
	// virtual void PrevUpdate() override; 
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	void CreateTileGrid(UINT _iRow, UINT _iCol); // 타일 격자 생성

	UINT GetCol() { return m_iCol; }
	UINT GetRow() { return m_iRow; }

	UINT GetTileMapWidth() { return m_iTileMapWidth; }
	UINT GetTileMapHeight() { return m_iTileMapHeight; }


	// 콜라이더 영역 최적화 하기.
	void OptimizationTileCollider();
	void GetEndIdxOfRectArea(int **_grid, int _startX, int _startY, int& _endX, int& _endY);


public:
	CLONE_DISABLE(CTileMap);
public:
	CTileMap() = delete;	
	CTileMap(E_GroupType _eGroupType);
	virtual ~CTileMap() override;
};