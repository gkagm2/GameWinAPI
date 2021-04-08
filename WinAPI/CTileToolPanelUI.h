#pragma once
#include "CPanelUI.h"
class CTileToolPanelUI : public CPanelUI
{
private:
	float m_fSelectedTileIdx;
	E_TileType m_eSelectedTileType;
	CTexture* m_pTileTexture;
	wstring m_strTexturePath;

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
		m_fSelectedTileIdx = _tileIdx;
		m_eSelectedTileType = _eTileType;
		m_pTileTexture = _pTileTexture;
		m_strTexturePath = _strTexturePath;
	}
	int GetSelectedTileIdx() { return m_fSelectedTileIdx; }
	E_TileType GetSelectedTileType() { return m_eSelectedTileType; }
	CTexture* GetTileTexture() { return m_pTileTexture; }
	const wstring& GetTexturePath() { return m_strTexturePath; }

public:
	CLONE(CTileToolPanelUI);

public:
	CTileToolPanelUI() = delete;
	CTileToolPanelUI(E_GroupType _eGroupType);
	virtual ~CTileToolPanelUI() override;
};