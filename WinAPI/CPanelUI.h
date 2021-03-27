#pragma once
#include "CUI.h"
class CPanelUI : public CUI
{
private:
	float m_fTopPadding;
	float m_fBottomPadding;
	float m_fLeftPadding;
	float m_fRightPadding;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	void SetPadding(float _fLeft, float _fTop, float _fRight, float _fBottom);
public:
	CPanelUI() = delete;
	CPanelUI(E_GroupType _eGroupType);
	virtual ~CPanelUI() override;
};