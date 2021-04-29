#pragma once
#include "CUI.h"
class CImageUI;
class CGTA_Player;
class CTextUI;
class CGTA_PlayerUI : public CUI
{
private:
	CImageUI* m_pWeaponImage;
	vector<CImageUI*> m_vecLifeImages;
	float m_fLife;

	CTextUI* m_pAmmoTextUI;
	CTextUI* m_pMoneyTextUI;

	CGTA_Player* m_pPlayer;
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	CImageUI* GetWeaponImage() { return m_pWeaponImage; }
	vector<CImageUI*>& GetLifeImage() { return m_vecLifeImages; }

	void SetLifeUI(float _fHP);
	void ChangeWeaponUI(const TWeaponInfo& _tWeaponInfo);

public:
	CLONE(CGTA_PlayerUI);
public:
	CGTA_PlayerUI() = delete;
	CGTA_PlayerUI(E_GroupType _eGroupType);
	virtual ~CGTA_PlayerUI() override;
};