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
#include "CCamera2D.h"
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CGTA_AI.h"

CGTA_Character::CGTA_Character(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_tInfo{},
	m_bIsDrive(false),
	m_fAttackCoolTime(0.f),
	m_fAttackMaxCoolTime(0.f),
	m_fStunCoolTime(0.f),
	m_fStunMaxCoolTime(10.f),
	m_fDeadCoolTime(0.f),
	m_fDeadMaxCoolTime(20.f),
	m_pVehicle(nullptr),
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
	m_vecWeapon[(UINT)E_WeaponType::FIST].second.fShootCoolTime = 0.7f;
}

CGTA_Character::CGTA_Character(const CGTA_Character& _origin) :
	CObject(_origin),
	m_tInfo(_origin.m_tInfo),
	m_bIsDrive(false),
	m_fAttackCoolTime(0.f),
	m_fAttackMaxCoolTime(0.f),
	m_fStunCoolTime(0.f),
	m_fStunMaxCoolTime(10.f),
	m_fDeadCoolTime(0.f),
	m_fDeadMaxCoolTime(20.f),
	m_pVehicle(nullptr),
	m_pPunchDetector(nullptr),
	m_eCurWeaponType(E_WeaponType::FIST),
	m_eCharacterState(E_CharacterState::idle)
{
}

CGTA_Character::~CGTA_Character()
{
}
	
void CGTA_Character::Init()
{
	// Collider set
	CColliderRect* pCollider = new CColliderRect(this);
	pCollider->SetScale(Vector3(15.f, 15.f, 0.f));

	CRigidbody2D* pRigidbody = new CRigidbody2D(this);
	SetRigidbody(pRigidbody);

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
		if (GetCollider()->IsRender())
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

void CGTA_Character::State()
{

}

void CGTA_Character::Stun()
{
	m_fStunCoolTime += DeltaTime;
	if (m_fStunCoolTime >= m_fStunMaxCoolTime) {
		SetCharacterState(E_CharacterState::run);
		m_fStunCoolTime =0.f;
	}
}

void CGTA_Character::Attack()
{

}

void CGTA_Character::CreateAI()
{
	if (nullptr == m_pAI) {
		m_pAI = new CGTA_AI();
		m_pAI->m_pCharacter = this;
	}
}

void CGTA_Character::ActivePunchDetector(bool _bActive)
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
	fwrite(&fShootCoolTime, sizeof(float), 1, _pFile);
}

void TWeaponInfo::Load(FILE* _pFile)
{
	LoadWString(strName, _pFile);
	fread(&fSplashRange, sizeof(float), 1, _pFile);
	fread(&fDamage, sizeof(float), 1, _pFile);
	fread(&iBulletCnt, sizeof(int), 1, _pFile);
	fread(&bSplashDamage, sizeof(bool), 1, _pFile);
	fread(&bIsInfinite, sizeof(bool), 1, _pFile);
	fread(&fShootCoolTime, sizeof(float), 1, _pFile);
}


TWeaponInfo::TWeaponInfo() : 
	strName(L""), 
	fSplashRange(20.f), 
	fDamage(1.f), 
	iBulletCnt(0), 
	bSplashDamage(false), 
	bIsInfinite(false), 
	fShootCoolTime(1.f)
{
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
