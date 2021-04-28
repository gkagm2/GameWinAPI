#pragma once
#include "CUI.h"
class CTexture;
class CGTA_WeaponUI : public CUI
{
private:
	TWeaponInfo m_tWeaponInfo;
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	void ChangeWeaponUI(const TWeaponInfo& _tWeaponInfo);

public:
	CLONE(CGTA_WeaponUI);
public:
	CGTA_WeaponUI() = delete;
	CGTA_WeaponUI(E_GroupType _eGroupType);
	virtual ~CGTA_WeaponUI() override;
};