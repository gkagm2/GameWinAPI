#pragma once
#include "CObject.h"
#include "CPathFinding.h"

class CScene;
class CPathFinding;
class CTileMap : public CObject
{
private:
	UINT m_iRow; // Ÿ�� ��
	UINT m_iCol; // Ÿ�� ��

	UINT m_iTileMapWidth;
	UINT m_iTileMapHeight;

private:
	enum class E_VisitedState { not_visited, visited }; // ����ȭ�� ����

private:
	void CreatePathFinding();

public:
	// virtual void PrevUpdate() override; 
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	void CreateTileGrid(UINT _iRow, UINT _iCol); // Ÿ�� ���� ����

	UINT GetCol() { return m_iCol; }
	UINT GetRow() { return m_iRow; }

	UINT GetTileMapWidth() { return m_iTileMapWidth; }
	UINT GetTileMapHeight() { return m_iTileMapHeight; }

	TTilePos VectorToTilePos(const Vector2& _vPos);
	Vector2 TilePosToVector(const TTilePos& _tTilePos);


	// �ݶ��̴� ���� ����ȭ �ϱ�.
	void OptimizationTileCollider();
	void GetEndIdxOfRectArea(int **_grid, int _startX, int _startY, int& _endX, int& _endY);

public:
	CLONE_DISABLE(CTileMap);
public:
	CTileMap() = delete;	
	CTileMap(E_GroupType _eGroupType);
	virtual ~CTileMap() override;
};