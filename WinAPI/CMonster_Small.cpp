#include "stdafx.h"
#include "CMonster_Small.h"
#include "CCore.h"
#include "CTimeManager.h"
#include "CPlayer.h"
#include "CMissile.h"

#include "CCamera.h"

CMonster_Small::CMonster_Small(E_GroupType _eGroupType) :
	CMonster(_eGroupType)
{
	m_eUpgradeLevel = E_UpgradeLevelType::LEVEL1;
}

CMonster_Small::~CMonster_Small()
{
}

void CMonster_Small::Update()
{
	m_fMissileFireCoolTime += DeltaTime;
	if (m_fMissileFireCoolTime >= m_fMissileMaxFireCoolTime) {
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

	Move(1);
}

void CMonster_Small::OnCollisionEnter(CObject* _pOther)
{
	CMissile* pMissile = dynamic_cast<CMissile*>(_pOther);
	if (pMissile) {
		if (pMissile->GetGroupType() == E_GroupType::PLAYER_PROJECTILE) {
			m_bIsHit = true;
			SetScale(GetScale() + 20.0f);
			if (GetScale().x > m_vOriginalScale.x * 1.5f)
				DeleteObject(this);
			DeleteObject(pMissile);
		}
	}
}