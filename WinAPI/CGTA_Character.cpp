#include "stdafx.h"
#include "CGTA_Character.h"
#include "CResourceManager.h"
#include "CGTA_Player.h"
#include "CColliderRect.h"
#include "CTexture.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CKeyManager.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CCamera.h"
#include "CObject.h"
#include "CGTA_PunchDetector.h"

CGTA_Character::CGTA_Character(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_bIsDrive(false),
	m_fAttackCoolTime(0.f),
	m_fAttackMaxCoolTime(0.f),
	m_pVehicle(nullptr),
	m_vNozzlePos(GetUpVector() * 5.f),
	m_pPunchDetector(nullptr),
	m_eCurWeaponType(E_WeaponType::FIST),
	m_eCharacterState(E_CharacterState::idle)
{
	// weapon system
	int iSize = (int)E_WeaponType::END;
	m_vecWeapon.resize(iSize, std::make_pair(false, TWeaponInfo{}));
	m_vecWeapon[(UINT)E_WeaponType::FIST].second.strName = STR_NAME_Fist;
	m_vecWeapon[(UINT)E_WeaponType::FIST].first = true;
	m_vecWeapon[(UINT)E_WeaponType::FIST].second.bIsInfinite = true;
	m_vecWeapon[(UINT)E_WeaponType::FIST].second.fShootCoolTime = 1.f;

	m_pPunchDetector = new CGTA_PunchDetector(E_GroupType::PROJECTILE);
	m_pPunchDetector->Init();
}

CGTA_Character::CGTA_Character(const CGTA_Character& _origin) :
	CObject(_origin),
	m_bIsDrive(false),
	m_fAttackCoolTime(0.f),
	m_fAttackMaxCoolTime(0.f),
	m_pVehicle(nullptr),
	m_vNozzlePos(GetUpVector() * 5.f),
	m_pPunchDetector(nullptr),
	m_eCurWeaponType(E_WeaponType::FIST),
	m_eCharacterState(E_CharacterState::idle)
{
	m_pPunchDetector = _origin.m_pPunchDetector->Clone();
}

CGTA_Character::~CGTA_Character()
{
	if (nullptr != m_pPunchDetector)
		delete m_pPunchDetector;
}
	
void CGTA_Character::Init()
{
	
	CObject::Init();
}

void CGTA_Character::PrevUpdate()
{
	__super::PrevUpdate();
}

void CGTA_Character::Update()
{
}

void CGTA_Character::LateUpdate()
{
	__super::LateUpdate();
}

void CGTA_Character::Render(HDC _hDC)
{
	if (false == IsRender())
		return;

	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	if (GetTexture()) {
		if (nullptr != GetAnimator())
			GetAnimator()->Render(_hDC);
		else {
			POINT rPNT[3];

			Vector3 v1 = GetRectPoint(0);
			Vector3 v2 = GetRectPoint(1);
			Vector3 v3 = GetRectPoint(2);
			rPNT[0].x = (int)(vRenderPosition.x + GetRectPoint(0).x);
			rPNT[0].y = (int)(vRenderPosition.y + GetRectPoint(0).y);
			rPNT[1].x = (int)(vRenderPosition.x + GetRectPoint(1).x);
			rPNT[1].y = (int)(vRenderPosition.y + GetRectPoint(1).y);
			rPNT[2].x = (int)(vRenderPosition.x + GetRectPoint(2).x);
			rPNT[2].y = (int)(vRenderPosition.y + GetRectPoint(2).y);

			HBITMAP bitmap{};
			PlgBlt(_hDC, rPNT, GetTexture()->GetDC(), 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), bitmap, 8, 5);
		}
	}

	if (nullptr != GetCollider()) {
		GetCollider()->Render(_hDC);
	}
	//__super::Render(_hDC);
}

void CGTA_Character::OnCollisionEnter(CObject* _pOther)
{
}

void CGTA_Character::OnCollisionStay(CObject* _pOther)
{
}

void CGTA_Character::OnCollisionExit(CObject* _pOther)
{
}

void CGTA_Character::Attack()
{

}

void TWeaponInfo::Save(FILE* _pFile)
{
	SaveWString(strName, _pFile);
	fwrite(&fSplashRange, sizeof(float), 1, _pFile);
	fwrite(&fDamage, sizeof(float), 1, _pFile);
	fwrite(&iBulletCnt, sizeof(int), 1, _pFile);
	fwrite(&bSplashDamage, sizeof(bool), 1, _pFile);
	fwrite(&bIsInfinite, sizeof(bool), 1, _pFile);
	fwrite(&fShootCoolTime, sizeof(bool), 1, _pFile);
}

void TWeaponInfo::Load(FILE* _pFile)
{
	LoadWString(strName, _pFile);
	fread(&fSplashRange, sizeof(float), 1, _pFile);
	fread(&fDamage, sizeof(float), 1, _pFile);
	fread(&iBulletCnt, sizeof(int), 1, _pFile);
	fread(&bSplashDamage, sizeof(bool), 1, _pFile);
	fread(&bIsInfinite, sizeof(bool), 1, _pFile);
	fread(&fShootCoolTime, sizeof(bool), 1, _pFile);
}

TWeaponInfo::TWeaponInfo(const TWeaponInfo& _other) :
	strName(_other.strName),
	fSplashRange(_other.fSplashRange),
	fDamage(_other.fDamage),
	iBulletCnt(_other.iBulletCnt),
	bSplashDamage(_other.bSplashDamage),
	bIsInfinite(_other.bIsInfinite),
	fShootCoolTime(_other.fShootCoolTime)
{
}

void TCharacterInfo::Save(FILE* _pFile)
{
	fwrite(&fHp, sizeof(float), 1, _pFile);
	fwrite(&fArmor, sizeof(float), 1, _pFile);
}

void TCharacterInfo::Load(FILE* _pFile)
{
	fread(&fHp, sizeof(float), 1, _pFile);
	fread(&fArmor, sizeof(float), 1, _pFile);
}
