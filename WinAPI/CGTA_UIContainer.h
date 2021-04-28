#pragma once
#include "CUI.h"
class CGTA_WeaponUI;
class CGTA_UIContainer : public CUI
{
private:
	CGTA_WeaponUI* m_pWeaponUI;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	CGTA_WeaponUI* GetWeaponUI() { return m_pWeaponUI; }

public:
	CLONE_DISABLE(CGTA_UIContainer);
public:
	CGTA_UIContainer() = delete;
	CGTA_UIContainer(E_GroupType _eGroupType);
	virtual ~CGTA_UIContainer() override;
};