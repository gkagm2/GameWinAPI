#pragma once
#include "CUI.h"

class CTexture;
class CTile;
class CImageUI : public CUI // Tile Image로 사용한다
{
private:
	const static int g_iTileSize;

private:
	Vector2 m_vLT; // image's left top position
	int m_iTileIdx;
	E_TileType m_eTileType;
	wstring m_strTexturePath;
	
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	//virtual void OnPointerDown() override;
	//virtual void OnPointerUp() override;
	virtual void OnPointerClick() override;

public:
	void SetLT(Vector2 _vLeftTopPos) { m_vLT = _vLeftTopPos; }
	Vector2 GetLT() { return m_vLT;	}

	void SetTileIdx(int _iTileIdx){ m_iTileIdx = _iTileIdx;}
	int GetTileIdx() { return m_iTileIdx; }
	void SetTileType(E_TileType _eTileType) { m_eTileType = _eTileType; }
	E_TileType GetTileType() { return m_eTileType; }
	void SetTexturePath(const wstring& _strTexturePath) { m_strTexturePath = _strTexturePath; }
	const wstring& GetTexturePath() { return m_strTexturePath; }

public:
	CLONE(CImageUI);
public:
	CImageUI() = delete;
	CImageUI(E_GroupType _eGroupType);
	virtual ~CImageUI() override;
};