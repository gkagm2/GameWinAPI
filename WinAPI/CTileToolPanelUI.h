#pragma once
#include "CPanelUI.h"
class CTileToolPanelUI : public CPanelUI
{
private:
	int m_iSelectedTileIdx;
	E_TileType m_eSelectedTileType;
	CTexture* m_pTileTexture;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	virtual void OnPointerDown() override;
	virtual void OnPointerUp() override;
	//virtual void OnPointerClick() override;

public:
	void SetSelectedTile(int _tileIdx, E_TileType _eTileType, CTexture* _pTileTexture, wstring _strTexturePath) {
		m_iSelectedTileIdx = _tileIdx;
		m_eSelectedTileType = _eTileType;
		m_pTileTexture = _pTileTexture;
	}
	int GetSelectedTileIdx() { return m_iSelectedTileIdx; }
	E_TileType GetSelectedTileType() { return m_eSelectedTileType; }
	CTexture* GetTileTexture() { return m_pTileTexture; }

public:
	CLONE(CTileToolPanelUI);

public:
	CTileToolPanelUI() = delete;
	CTileToolPanelUI(E_GroupType _eGroupType);
	virtual ~CTileToolPanelUI() override;
};