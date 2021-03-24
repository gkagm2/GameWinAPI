#pragma once
#include "CUI.h"
class CPanelUI : public CUI
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;
public:
	CPanelUI() = delete;
	CPanelUI(E_GroupType _eGroupType);
	virtual ~CPanelUI() override;
};