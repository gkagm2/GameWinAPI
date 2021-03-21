#pragma once
#include "CObject.h"
class CTileMap : public CObject
{
private:
	UINT m_iRow; // Ÿ�� ��
	UINT m_iCol; // Ÿ�� ��

	UINT m_iTileMapWidth;
	UINT m_iTileMapHeight;

public:
	//virtual void PrevUpdate(); 
	virtual void Update() override;
	//virtual void LateUpdate();
	virtual void Render(HDC _hDC) override;

public:
	void CreateTileGrid(UINT _iRow, UINT _iCol); // Ÿ�� ���� ����

	UINT GetCol() { return m_iCol; }
	UINT GetRow() { return m_iRow; }

	UINT GetTileMapWidth() { return m_iTileMapWidth; }
	UINT GetTileMapHeight() { return m_iTileMapHeight; }

public:
	CTileMap() = delete;	
	CTileMap(E_GroupType _eGroupType);
	virtual ~CTileMap() override;
};