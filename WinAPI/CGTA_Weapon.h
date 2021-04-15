#pragma once

struct TWeaponInfo {
	wstring strName;
	float fSplashRange;
	float fDamage;
	int iBulletCnt;
	bool bSplashDamage;
	bool bIsInfinite;
	float fShootCoolTime;
	TWeaponInfo() : strName(L""), fSplashRange(20.f), fDamage(1.f), iBulletCnt(0), bSplashDamage(false),  bIsInfinite(false), fShootCoolTime(1.f){}
};

struct TCharacterInfo {
	float fHp;
	float fArmor;

	TCharacterInfo() : fHp(10.f), fArmor(0.f) {}
};

class CGTA_Weapon
{
private:	
	E_WeaponType m_eCurWeaponType;
	vector<std::pair<bool, TWeaponInfo> > m_vecWeapon; // true : allow, false : not allow

public:
	void ChangeNextWeapon();
	void ChangePrevWeapon();
	void Init();
	E_WeaponType GetCurWeaponType() { return m_eCurWeaponType; }
	TWeaponInfo& GetCurWeaponInfo() { return m_vecWeapon[(UINT)m_eCurWeaponType].second; }
	void SetWeaponState(bool _bAllow, E_WeaponType _eWeaponType) { m_vecWeapon[(UINT)_eWeaponType].first = _bAllow; }
	void SetWeaponInfo(E_WeaponType _eWeaponType, const TWeaponInfo& _tWeaponInfo) { m_vecWeapon[(UINT)_eWeaponType].second = _tWeaponInfo; }

public:
	CGTA_Weapon();
	virtual ~CGTA_Weapon() final;
};