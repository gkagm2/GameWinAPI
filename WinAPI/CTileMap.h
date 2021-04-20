#pragma once
#include "CObject.h"
#include "CPathFinding.h"

class CScene;
class CPathFinding;
class CTileMap : public CObject
{
private:
	UINT m_iRow; // 타일 행
	UINT m_iCol; // 타일 열

	UINT m_iTileMapWidth;
	UINT m_iTileMapHeight;

private:
	enum class E_VisitedState { not_visited, visited }; // 최적화에 사용됨

private:
	void CreatePathFinding();

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

	TTilePos VectorToTilePos(const Vector2& _vPos);
	Vector2 TilePosToVector(const TTilePos& _tTilePos);


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