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

#include "CAnimator.h"
#include "CRigidbody2D.h"

CPlayer::CPlayer(E_GroupType _eGroupType = E_GroupType::DEFAULT) :
	CObject(_eGroupType),
	m_iHp(9999),
	m_fSpeed(300.0f),
	m_fMissileSpeed(600.0f),
	m_eUpgradeLevel(E_UpgradeLevelType::LEVEL1),
	m_fFireCoolTime(0.0f),
	m_fFireMaxCoolTime(0.1f)
{
	// 플레이어 리지드바디 추가
	CRigidbody2D* pRigidbody = new CRigidbody2D(this);
	pRigidbody->SetMass(10.0f);
	pRigidbody->UseGravity(false);
	pRigidbody->SetDrag(3.0f);
}

CPlayer::~CPlayer()
{

}

void CPlayer::Update()
{
	Vector3 vMoveVec(0, 0, 0);
	float fForce = 200.0f;
	// Force
	if (InputKeyHold(E_Key::RBUTTON)) {
	}
	if (InputKeyHold(E_Key::LEFT)) {// 왼쪽 키가 눌렸다면
		GetRigidbody()->AddForce(Vector3(-fForce, 0, 0));

		GetAnimator()->PlayAnimation(L"WALK_LEFT", E_AnimationPlayType::LOOP);
	}
	if (InputKeyHold(E_Key::RIGHT)) {
		GetRigidbody()->AddForce(Vector3(fForce, 0, 0));

		GetAnimator()->PlayAnimation(L"WALK_RIGHT", E_AnimationPlayType::LOOP);
	}
	if (InputKeyHold(E_Key::UP)) {
		GetRigidbody()->AddForce(Vector3(0, -fForce, 0));

		GetAnimator()->PlayAnimation(L"WALK_UP", E_AnimationPlayType::LOOP);
	}
	if (InputKeyHold(E_Key::DOWN)) {
		GetRigidbody()->AddForce(Vector3(0, fForce, 0));

		GetAnimator()->PlayAnimation(L"WALK_DOWN", E_AnimationPlayType::LOOP);
	}
	if (InputKeyRelease(E_Key::LEFT) ) {// 왼쪽 키가 눌렸다면
		GetAnimator()->PlayAnimation(L"IDLE_LEFT", E_AnimationPlayType::LOOP);
	}
	if (InputKeyRelease(E_Key::RIGHT)) {
		GetAnimator()->PlayAnimation(L"IDLE_RIGHT", E_AnimationPlayType::LOOP);
	}
	if (InputKeyRelease(E_Key::UP)) {
		GetAnimator()->PlayAnimation(L"IDLE_UP", E_AnimationPlayType::LOOP);
	}
	if (InputKeyRelease(E_Key::DOWN)) {
		GetAnimator()->PlayAnimation(L"IDLE_DOWN", E_AnimationPlayType::LOOP);
	}

	vMoveVec.Normalized();

	Vector3 vPosition = GetPosition();
	SetPosition(vPosition + vMoveVec * m_fSpeed * DeltaTime);
	
	if (vPosition.y > (float)CCore::GetInstance()->GetResolution().y) {
		vPosition.y = 0;
		SetPosition(vPosition);
	}
	if (vPosition.y < 0) {
		vPosition.y = (float)CCore::GetInstance()->GetResolution().y;
		SetPosition(vPosition);
	}
	if (vPosition.x > (float)CCore::GetInstance()->GetResolution().x) {
		vPosition.x = 0;
		SetPosition(vPosition);
	}
	if (vPosition.x < 0) {
		vPosition.x = (float)CCore::GetInstance()->GetResolution().x;
		SetPosition(vPosition);
	}

	if (GetAsyncKeyState(0x31) & 0x8000) { // num 1
		m_eUpgradeLevel = E_UpgradeLevelType::LEVEL1;
	}
	if (GetAsyncKeyState(0x32) & 0x8000) { // num 2
		m_eUpgradeLevel = E_UpgradeLevelType::LEVEL2;
	}
	if (GetAsyncKeyState(0x33) & 0x8000) { // num 3
		m_eUpgradeLevel = E_UpgradeLevelType::LEVEL3;
	}
	if (GetAsyncKeyState(0x34) & 0x8000) { // num4
 		m_eUpgradeLevel = E_UpgradeLevelType::LEVEL4;
	}

	// Mouse Click test code
	if (InputKeyPress(E_Key::LBUTTON)) {
		Vector2 clickPos = MousePosition;
		clickPos = MainCamera->GetScreenToWorldPosition(clickPos);
		Vector3 vNozzlePosition(vPosition.x, vPosition.y - GetScale().y / 2.0f);
		Vector2 directPos = clickPos - vNozzlePosition;
		directPos.Normalized();
		directPos.y *= -1;
		CreateMissile(m_fMissileSpeed, vNozzlePosition, directPos);
	}

	m_fFireCoolTime += DeltaTime;
	if (InputKeyPress(E_Key::SPACE)) {
		FireMissile();
		m_fFireCoolTime = 0.0f;
	}
		
	if (m_fFireCoolTime >= m_fFireMaxCoolTime) {
		if (InputKeyHold(E_Key::SPACE))
 			FireMissile();
		m_fFireCoolTime = 0.0f;
	}
}

void CPlayer::LateUpdate()
{
	CObject::LateUpdate();
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
				DestroyObject(this);
			DestroyObject(pMissile);
		}
	}
	CMonster* pMonster = dynamic_cast<CMonster*>(_pOther);
	if (pMonster) {
		if (pMonster->GetGroupType() == E_GroupType::MONSTER) {
			--m_iHp;
			if (m_iHp <= 0)
				DestroyObject(this);
		}
	}
}

void CPlayer::CreateMissile(float _fSpeed, Vector3 _vPosition, Vector2 _vDirVec) {
	CreateMissile(_fSpeed, _vPosition, Vector3(_vDirVec.x, _vDirVec.y));
}

void CPlayer::CreateMissile(float _fSpeed, Vector3 _vNozzlePosition, Vector3 _vDirVec)
{
	CMissile* pMissile = new CMissile(E_GroupType::PLAYER_PROJECTILE);
	pMissile->SetObjectName(L"Player Missile");
	pMissile->SetSpeed(_fSpeed);
	pMissile->SetPosition(_vNozzlePosition);
	pMissile->SetScale(Vector3(10.0f, 10.0f));
	pMissile->SetDirectionVector(_vDirVec);
	
	// 미사일 충돌 컴포넌트 생성 및 추가
	CColliderCircle* pMissileCircleCollider = new CColliderCircle(pMissile);
	CreateObject(pMissile);
}

void CPlayer::FireMissile() {
	Vector3 vPosition = GetPosition();
	Vector3 vDirUp = Vector3(0, 1, 0);
	switch (m_eUpgradeLevel) {
	case E_UpgradeLevelType::LEVEL1:
	{
		Vector3 vNozzlePosition(vPosition.x, vPosition.y - (GetScale().y / 2.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, vDirUp);
	}
		break;
	case E_UpgradeLevelType::LEVEL2:
	{
		Vector3 vNozzlePosition(vPosition.x, vPosition.y);

		Vector3 vLeftNozzlePosition(vNozzlePosition.x - GetScale().x / 2.0f + 5.0f, vNozzlePosition.y);
		Vector3 vRightNozzlePosition(vNozzlePosition.x + GetScale().x / 2.0f -5.0f, vNozzlePosition.y);
		CreateMissile(m_fMissileSpeed, vLeftNozzlePosition, vDirUp);
		CreateMissile(m_fMissileSpeed, vRightNozzlePosition, vDirUp);
	}
		break;
	case E_UpgradeLevelType::LEVEL3:
	{
		Vector3 vNozzlePosition(vPosition.x, vPosition.y);
		Vector3 vLeftNozzlePosition(vNozzlePosition.x - GetScale().x / 2.0f +5.0f, vNozzlePosition.y);
		Vector3 vRightNozzlePosition(vNozzlePosition.x + GetScale().x / 2.0f -5.0f, vNozzlePosition.y);
		CreateMissile(m_fMissileSpeed, vLeftNozzlePosition, Rotate(vDirUp, 15.0f));
		CreateMissile(m_fMissileSpeed, vRightNozzlePosition, Rotate(vDirUp, - 15.0f));
		vNozzlePosition.Set(vPosition.x, vPosition.y - GetScale().y / 2.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, vDirUp);
	}
		break;
	case E_UpgradeLevelType::LEVEL4:
	{
		Vector3 vNozzlePosition(vPosition.x, vPosition.y - GetScale().y / 2.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, Rotate(vDirUp, - 15.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, Rotate(vDirUp, + 15.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, Rotate(vDirUp, - 30.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, Rotate(vDirUp, + 30.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, Rotate(vDirUp, - 45.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, Rotate(vDirUp, + 45.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, vDirUp);
	}
		break;
	default:
		break;
	}
}
