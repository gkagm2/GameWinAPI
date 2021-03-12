#include "stdafx.h"
#include "CMonster.h"
#include "CTimeManager.h"
#include "CCore.h"
#include "CMissile.h"
#include "CColliderCircle.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CPlayer.h"
enum class E_MissileType;
CMonster::CMonster(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_fSpeed(1.0f),
	m_fDirection(1.0f),
	m_fRange(800.0f),
	m_vStartPosition(0, 0, 0),
	m_fRatioX(0),
	m_fRatioY(0),
	m_eUpgradeLevel(E_UpgradeLevelType::LEVEL1),
	m_fHitEffectMaxCoolTime(0.4f),
	m_fHitEffectCoolTime(0.0f),
	m_bIsHit(false),
	m_vOriginalScale(GetScale()),
	m_fMissileFireCoolTime(0.0f),
	m_fMissileMaxFireCoolTime(3.0f),
	m_fMissileSpeed(400.0f),
	m_pTargetObj(nullptr)
{
	m_vOriginalScale = GetScale();
}

CMonster::~CMonster()
{
}

void CMonster::Update()
{
	// Shoot Missile 
	m_fMissileFireCoolTime += DeltaTime;
	if (m_fMissileFireCoolTime >= m_fMissileMaxFireCoolTime) {
		float fRandomMaxCoolTime = (float)((rand() % 2) + 0.2f);
		int eRandomFireLevel = rand() % (int)E_UpgradeLevelType::END;
		m_eUpgradeLevel = (E_UpgradeLevelType)eRandomFireLevel;
		m_fMissileMaxFireCoolTime = fRandomMaxCoolTime;
		FireMissile();

		m_fMissileFireCoolTime = 0.0f;
	}
	
	// Hit Effect
	if (m_bIsHit) {
 		Vector3 vScale = GetScale();
		vScale = vScale - 80.0f * DeltaTime;
		if (vScale.x <= m_vOriginalScale.x) {
			vScale = m_vOriginalScale;
			m_bIsHit = false;
		}
		SetScale(vScale);
	}

	// Move
	Move(2);
}

void CMonster::OnCollisionEnter(CObject* _pOther)
{
  	CMissile* pMissile = dynamic_cast<CMissile*>(_pOther);
	if (pMissile) {
		E_GroupType groupType = pMissile->GetGroupType();

		m_bIsHit = true;
		SetScale(GetScale() + 30);
		if (GetScale().x > m_vOriginalScale.x * 4.0f)
			DestroyObject(this);
	}
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther);
	if (pPlayer) {
		E_GroupType  groupType = pPlayer->GetGroupType();
		DestroyObject(this);
	}
}

void CMonster::CreateMissile(float _fSpeed, Vector3 _vNozzlePosition, float _fDegree, E_MissileType _eMissileType = E_MissileType::NORMAL)
{
	CMissile* pMissile = new CMissile(E_GroupType::MONSTER_PROJECTILE);
	pMissile->SetMissileType(_eMissileType);
	pMissile->SetSpeed(_fSpeed);
	pMissile->SetPosition(_vNozzlePosition);
	pMissile->SetScale(Vector3(10.0f, 10.0f));
	pMissile->SetDirectionDegree(_fDegree);

	switch (_eMissileType) {
	case E_MissileType::NORMAL:
		pMissile->SetObjectName(L"Monster Misisle");
		break;
	case E_MissileType::GUIDED:
	{
		pMissile->SetObjectName(L"Monster Guided Missile");
		if (nullptr != m_pTargetObj)
			pMissile->SetTargetObject(m_pTargetObj);
	}
		break;
	}

	// 미사일 충돌 컴포넌트 생성 및 추가
	CColliderCircle* pMissileCircleCollider = new CColliderCircle(pMissile);
	CreateObject(pMissile);
}

void CMonster::FireMissile()
{
	Vector3 vPosition = GetPosition();
	Vector3 vNozzlePosition(vPosition.x, vPosition.y + ScaleY() / 2.0f);

	assert(nullptr != m_pTargetObj);
	Vector3 vMonsterPos = GetPosition();
	Vector3 vTargetPos = m_pTargetObj->GetPosition();
	Vector3 vDir = vTargetPos - vMonsterPos;
	vDir.y *= -1;
	float fDegree = CMyMath::VectorToDegree(vDir);

	switch (m_eUpgradeLevel) {
	case E_UpgradeLevelType::LEVEL1:
	{
		CreateMissile(m_fMissileSpeed, vNozzlePosition, fDegree, E_MissileType::GUIDED);
	}
	break;
	case E_UpgradeLevelType::LEVEL2:
	{
		Vector3 vLeftNozzlePosition(vNozzlePosition.x - ScaleX() / 2.0f, vNozzlePosition.y);
		Vector3 vRightNozzlePosition(vNozzlePosition.x + ScaleX() / 2.0f, vNozzlePosition.y);
		CreateMissile(m_fMissileSpeed, vLeftNozzlePosition, fDegree);
		CreateMissile(m_fMissileSpeed, vRightNozzlePosition, fDegree);
	}
	break;
	case E_UpgradeLevelType::LEVEL3:
	{
		Vector3 vLeftNozzlePosition(vNozzlePosition.x - ScaleX() / 2.0f, vNozzlePosition.y);
		Vector3 vRightNozzlePosition(vNozzlePosition.x + ScaleX() / 2.0f, vNozzlePosition.y);
		CreateMissile(m_fMissileSpeed, vRightNozzlePosition, fDegree + 15.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, fDegree);
		CreateMissile(m_fMissileSpeed, vLeftNozzlePosition, fDegree - 15.0f);
	}
	break;
	case E_UpgradeLevelType::LEVEL4:
	{
		CreateMissile(m_fMissileSpeed, vNozzlePosition, -90.0f - 15.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, -90.0f + 15.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, -90.0f - 30.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, -90.0f + 30.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, -90.0f - 45.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, -90.0f + 45.0f);
		CreateMissile(m_fMissileSpeed, vNozzlePosition, -90.0f);
	}
	break;
	default:
		break;
	}
}

void CMonster::Move(int _iType = 0)
{
	_iType = _iType < 0 ? max(_iType, 0) : min(_iType, 3);

	switch (_iType) {
	case 0:
		// just static
		break;
	case 1:
		// move1
	{
		Vector3 startPosition = m_vStartPosition;
		m_fRatioX += DeltaTime * m_fSpeed;
		float offsetX = cosf(m_fRatioX) * m_fRange / 2.0f;
		SetPosition(m_vStartPosition.x + offsetX, m_vStartPosition.y);
	}
		break;
	case 2:
		// move2
	{
		Vector3 targetPosition = m_pTargetObj->GetPosition();
		Vector3 vMovedPosition = targetPosition;
		m_fRatioX += DeltaTime * m_fSpeed;
		m_fRatioY += DeltaTime * 3.0f;
		float offsetX = cosf(m_fRatioX) * m_fRange / 2.0f;
		float offsetY = sinf(m_fRatioY) * 150.0f / 2.0f;
		SetPosition(m_vStartPosition.x + offsetX, m_vStartPosition.y + offsetY);
	}
		break;
	case 3:
		break;
	}
}
