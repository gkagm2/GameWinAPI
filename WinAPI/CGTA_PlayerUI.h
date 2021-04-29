#pragma once
#include "CUI.h"
class CImageUI;
class CGTA_PlayerUI : public CUI
{
private:
	TWeaponInfo m_tWeaponInfo;
	Vector3 m_vWeaponPos;

	CImageUI* m_pWeaponImage;
	vector<CImageUI*> m_vecLifeImages;
	float m_fLife;

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