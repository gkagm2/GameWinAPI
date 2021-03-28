#pragma once
#include "CObject.h"
class CScene;
class CTileMap : public CObject
{
private:
	UINT m_iRow; // Ÿ�� ��
	UINT m_iCol; // Ÿ�� ��

	UINT m_iTileMapWidth;
	UINT m_iTileMapHeight;
	

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

public:
	CLONE_DISABLE(CTileMap);
public:
	CTileMap() = delete;	
	CTileMap(E_GroupType _eGroupType);
	virtual ~CTileMap() override;
};