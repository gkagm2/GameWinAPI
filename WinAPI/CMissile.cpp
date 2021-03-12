#include "stdafx.h"
#include "CMissile.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CScene.h"

CMissile::CMissile(E_GroupType e_GroupType) :
	CObject(e_GroupType),
	m_fSpeed(100.0f),
	m_eMissileType(E_MissileType::NORMAL),
	m_fDirDegree(135),
	m_fLivingMaxTime(3.0f),
	m_fLivingTime(0.0f),
	m_pTargetObject(nullptr)
{
}

CMissile::~CMissile()
{
}

void CMissile::SetMissileType(E_MissileType _eMissileType)
{
	m_eMissileType = _eMissileType;
	switch (m_eMissileType) {
	case E_MissileType::GUIDED:
		m_fLivingMaxTime = 6.0f;
		break;
	}
}

void CMissile::SetTargetObject(CObject* _pTargetObject)
{
	m_pTargetObject = _pTargetObject;
}

void CMissile::Update()
{
	m_fLivingTime += DeltaTime;
	if (m_fLivingTime >= m_fLivingMaxTime) {
		DestroyObject(this);
		return;
	}

	switch (m_eMissileType) {
	case E_MissileType::NORMAL:
	{
		Vector3 vPosition = GetPosition();
		float fRadian = CMyMath::DegreeToRadian(m_fDirDegree);
		SetPosition(vPosition.x + cosf(fRadian) * m_fSpeed * DeltaTime, vPosition.y - sinf(fRadian) * m_fSpeed * DeltaTime);
	}
		break;
	case E_MissileType::GUIDED:
	{
		if (nullptr == m_pTargetObject)
			break;
		Vector3 vTargetPos = m_pTargetObject->GetPosition();
		Vector3 vPosition = GetPosition();
		Vector3 vDirVec = vTargetPos - vPosition;
		vDirVec.Normalized();
		vDirVec.y *= -1;
		float fRadian = CMyMath::VectorToRadian(vDirVec);

		SetPosition(vPosition.x + cosf(fRadian) * m_fSpeed * DeltaTime, vPosition.y - sinf(fRadian) * m_fSpeed * DeltaTime);
	}

		break;
	case E_MissileType::UPGRADE:

		break;
	}
	// check all of monster object from 
}