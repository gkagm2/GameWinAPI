#pragma once
#include "CUI.h"
class CGTA_PlayerUI;
class CGTA_UIContainer : public CUI
{
private:
	CGTA_PlayerUI* m_pWeaponUI;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	CGTA_PlayerUI* GetPlayerUI() { return m_pWeaponUI; }

public:
	CLONE_DISABLE(CGTA_UIContainer);
public:
	CGTA_UIContainer() = delete;
	CGTA_UIContainer(E_GroupType _eGroupType);
	virtual ~CGTA_UIContainer() override;
};