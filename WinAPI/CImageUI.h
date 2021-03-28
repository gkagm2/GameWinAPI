#pragma once
#include "CUI.h"

class CTexture;
class CImageUI : public CUI
{
private:
	const static int g_iTileSize;

private:
	Vector2 m_vLT; // image's left top position
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

public:
	CLONE(CImageUI);
public:
	CImageUI() = delete;
	CImageUI(E_GroupType _eGroupType);
	virtual ~CImageUI() override;
};