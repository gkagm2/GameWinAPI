#include "stdafx.h"
#include "CGTA_Weapon.h"

CGTA_Weapon::CGTA_Weapon()
{
	Init();
}

CGTA_Weapon::~CGTA_Weapon()
{
}

void CGTA_Weapon::ChangeNextWeapon()
{
	int iNextWeaponIdx = (int)m_eCurWeaponType + 1;
	while (true) {
		if (iNextWeaponIdx >= (int)E_WeaponType::END)
			iNextWeaponIdx = 0;

		if (true == m_vecWeapon[iNextWeaponIdx].first) {
			m_eCurWeaponType = (E_WeaponType)iNextWeaponIdx;
			break;
		}
		++iNextWeaponIdx;
	}
}

void CGTA_Weapon::ChangePrevWeapon()
{
	int iPrevWeaponIdx = (int)m_eCurWeaponType - 1;
	while (true) {
		if (iPrevWeaponIdx < 0)
			iPrevWeaponIdx = (int)E_WeaponType::END - 1;
		
		if (true == m_vecWeapon[iPrevWeaponIdx].first) {
			m_eCurWeaponType = (E_WeaponType)iPrevWeaponIdx;
			break;
		}
		--iPrevWeaponIdx;
	}
}

void CGTA_Weapon::Init()
{
	// weapon system
	int iSize = (int)E_WeaponType::END;
	m_vecWeapon.resize(iSize, std::make_pair(false,TWeaponInfo{}));
	m_vecWeapon[(UINT)E_WeaponType::FIST].second.strName = STR_NAME_Fist;
	m_vecWeapon[(UINT)E_WeaponType::FIST].first =  true;
	m_vecWeapon[(UINT)E_WeaponType::FIST].second.bIsInfinite = true;
	m_vecWeapon[(UINT)E_WeaponType::FIST].second.fShootCoolTime = 1.f;

	m_vecWeapon[(UINT)E_WeaponType::ROCKET_LAUNCHER].second.strName = STR_NAME_RocketLauncher;
	m_vecWeapon[(UINT)E_WeaponType::ROCKET_LAUNCHER].second.fShootCoolTime = 2.5f;


	m_vecWeapon[(UINT)E_WeaponType::PISTOL].second.strName = STR_NAME_Pistol;
	m_vecWeapon[(UINT)E_WeaponType::PISTOL].second.fShootCoolTime = 1.f;

	m_vecWeapon[(UINT)E_WeaponType::SHOTGUN].second.strName = STR_NAME_Shotgun;
	m_vecWeapon[(UINT)E_WeaponType::SHOTGUN].second.fShootCoolTime = 1.7f;

	m_vecWeapon[(UINT)E_WeaponType::SUBMACHINE_GUN].second.strName = STR_NAME_SubmachineGun;
	m_vecWeapon[(UINT)E_WeaponType::SUBMACHINE_GUN].second.fShootCoolTime = 0.3f;
}