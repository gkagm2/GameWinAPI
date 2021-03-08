#include "stdafx.h"
#include "CPlayer.h"

#include "CKeyManager.h"
#include "CTimeManager.h"

#include "CSceneManager.h"
#include "CScene.h"

#include "CTexture.h"

#include "CCore.h"

#include "CColliderCircle.h"
#include "CCollider.h"
#include "CCollisionManager.h"

#include "CMissile.h"
#include "CMonster.h"
#include "CCamera.h"

CPlayer::CPlayer(E_GroupType _eGroupType = E_GroupType::DEFAULT) :
	CObject(_eGroupType),
	m_iHp(10),
	m_fSpeed(300.0f),
	m_fMissileSpeed(600.0f),
	m_eUpgradeLevel(E_UpgradeLevelType::LEVEL1),
	m_fFireCoolTime(0.0f),
	m_fFireMaxCoolTime(0.1f)
{
}

CPlayer::~CPlayer()
{
}

/*
#define KEY_TAP(key) CKeyMgr::GetInst()->GetKeyState(key) == KSTATE::TAP
#define KEY_HOLD(key) CKeyMgr::GetInst()->GetKeyState(key) == KSTATE::HOLD
#define KEY_AWAY(key) CKeyMgr::GetInst()->GetKeyState(key) == KSTATE::AWAY
#define KEY_NONE(key) CKeyMgr::GetInst()->GetKeyState(key) == KSTATE::NONE
*/

void CPlayer::Update()
{	
	CKeyManager* keyMrg = CKeyManager::GetInstance();
	
	Vector3 vMoveVec(0, 0, 0);
	if (keyMrg->GetKeyState(E_Key::LEFT) == E_KeyState::HOLD) // 왼쪽 키가 눌렸다면
		vMoveVec.x -= 1;
	if (keyMrg->GetKeyState(E_Key::RIGHT) == E_KeyState::HOLD)
		vMoveVec.x += 1;
	if (keyMrg->GetKeyState(E_Key::UP) == E_KeyState::HOLD)
		vMoveVec.y -= 1;
	if (keyMrg->GetKeyState(E_Key::DOWN) == E_KeyState::HOLD)
		vMoveVec.y += 1;
	vMoveVec.Normalized();

	Vector3 vPosition = GetPosition();
	SetPosition(vPosition + vMoveVec * m_fSpeed * DeltaTime);

	
	if (GetAsyncKeyState(0x31) & 0x8000) {
		m_eUpgradeLevel = E_UpgradeLevelType::LEVEL1;
	}
	if (GetAsyncKeyState(0x32) & 0x8000) {
		m_eUpgradeLevel = E_UpgradeLevelType::LEVEL2;
	}
	if (GetAsyncKeyState(0x33) & 0x8000) {
		m_eUpgradeLevel = E_UpgradeLevelType::LEVEL3;
	}
	if (GetAsyncKeyState(0x34) & 0x8000) {
 		m_eUpgradeLevel = E_UpgradeLevelType::LEVEL4;
	}


	// Mouse Click test code
	POINT lpp;
	if (keyMrg->GetKeyState(E_Key::LBUTTON) == E_KeyState::PRESS) {
		if (GetCursorPos(&lpp)) {
			LONG ia = lpp.x;
			LONG ja = lpp.y;
			Vector3 clickPos(lpp.x, lpp.y);
			clickPos = CCamera::GetInstance()->GetScreenToWorldPosition(clickPos);
			Vector3 position = GetPosition();
			Vector3 playerPos = position;

			Vector3 resultPos = clickPos - playerPos;
			resultPos.y *= -1;
			float fDegree = CMyMath::VectorToDegree(resultPos);

			Vector3 vNozzlePosition(vPosition.x, vPosition.y - GetTexture()->GetHeight() / 2.0f);
			CreateMissile(m_fMissileSpeed, vNozzlePosition, fDegree);
		}
	}


	m_fFireCoolTime += DeltaTime;
	if (keyMrg->GetKeyState(E_Key::SPACE) == E_KeyState::PRESS) {
		FireMissile();
		m_fFireCoolTime = 0.0f;
	}
		
	if (m_fFireCoolTime >= m_fFireMaxCoolTime) {
		if (keyMrg->GetKeyState(E_Key::SPACE) == E_KeyState::HOLD)
			FireMissile();
		m_fFireCoolTime = 0.0f;
	}
}

void CPlayer::LateUpdate()
{
	HDC hdc = CCore::GetInstance()->GetDC();
	wchar_t strHpBuffer[255];
	swprintf_s(strHpBuffer, 255, L"HP : %d", m_iHp);
	TextOut(hdc, 10, 10, strHpBuffer, (int)wcslen(strHpBuffer));
}

void CPlayer::OnCollisionEnter(CObject* _pOther)
{
	CMissile* pMissile = dynamic_cast<CMissile*>(_pOther);
	if (pMissile) {
		if (pMissile->GetGroupType() == E_GroupType::MONSTER_PROJECTILE) {
			--m_iHp;
			if (m_iHp <= 0)
				DeleteObject(this);
		}
	}
	CMonster* pMonster = dynamic_cast<CMonster*>(_pOther);
	if (pMonster) {
		if (pMonster->GetGroupType() == E_GroupType::MONSTER) {
			--m_iHp;
			if (m_iHp <= 0)
				DeleteObject(this);
		}
	}
}

void CPlayer::CreateMissile(float _fSpeed, Vector3 _vNozzlePosition, float _fDirAngle)
{
	CMissile* pMissile = new CMissile(E_GroupType::PLAYER_PROJECTILE);
	pMissile->SetObjectName(L"Player Missile");
	pMissile->SetSpeed(_fSpeed);
	pMissile->SetPosition(_vNozzlePosition);
	pMissile->SetScale(Vector3(10.0f, 10.0f));
	pMissile->SetDirectionAngle(_fDirAngle);
	
	// 미사일 충돌 컴포넌트 생성 및 추가
	CColliderCircle* pMissileCircleCollider = new CColliderCircle(pMissile);
	CreateObject(pMissile);
}

void CPlayer::FireMissile() {
	Vector3 vPosition = GetPosition();
	switch (m_eUpgradeLevel) {
	case E_UpgradeLevelType::LEVEL1:
	{
		Vector3 vNozzlePosition(vPosition.x, vPosition.y - GetTexture()->GetHeight() / 2.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, 90.0f);
	}
		break;
	case E_UpgradeLevelType::LEVEL2:
	{
		Vector3 vNozzlePosition(vPosition.x, vPosition.y);

		Vector3 vLeftNozzlePosition(vNozzlePosition.x - GetTexture()->GetWidth() / 2.0f + 5.0f, vNozzlePosition.y);
		Vector3 vRightNozzlePosition(vNozzlePosition.x + GetTexture()->GetWidth() / 2.0f -5.0f, vNozzlePosition.y);
		CreateMissile(m_fMissileSpeed, vLeftNozzlePosition, 90.0f);
		CreateMissile(m_fMissileSpeed, vRightNozzlePosition, 90.0f);
	}
		break;
	case E_UpgradeLevelType::LEVEL3:
	{
		Vector3 vNozzlePosition(vPosition.x, vPosition.y);
		Vector3 vLeftNozzlePosition(vNozzlePosition.x - GetTexture()->GetWidth() / 2.0f +5.0f, vNozzlePosition.y);
		Vector3 vRightNozzlePosition(vNozzlePosition.x + GetTexture()->GetWidth() / 2.0f -5.0f, vNozzlePosition.y);
		CreateMissile(m_fMissileSpeed, vLeftNozzlePosition, 90.0f + 15.0f);
		CreateMissile(m_fMissileSpeed, vRightNozzlePosition, 90.0f - 15.0f);
		vNozzlePosition.Set(vPosition.x, vPosition.y - GetTexture()->GetHeight() / 2.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, 90.0f);
	}
		break;
	case E_UpgradeLevelType::LEVEL4:
	{
		Vector3 vNozzlePosition(vPosition.x, vPosition.y - GetTexture()->GetHeight() / 2.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, 90.0f - 15.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, 90.0f + 15.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, 90.0f - 30.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, 90.0f + 30.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, 90.0f - 45.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, 90.0f + 45.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, 90.0f);
	}
		break;
	default:
		break;
	}
}
