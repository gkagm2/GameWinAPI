#include "stdafx.h"
#include "CMissile.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CScene.h"

#include "CCore.h" //Test

CMissile::CMissile(E_GroupType e_GroupType) :
	CObject(e_GroupType),
	m_fSpeed(600.0f),
	m_eMissileType(E_MissileType::NORMAL),
	m_fLivingMaxTime(3.0f),
	m_fLivingTime(0.0f),
	m_pTargetObject(nullptr),
	m_testCoolTime(0.0f),
	m_testMaxCoolTime(0.1f),
	m_vPrevHeadDir{}
{
}

CMissile::~CMissile()
{
}

float CMissile::GetTargetAngle(const Vector3& _vPos, const Vector3& _vTargetPos)
{
	Vector3 vTargetPos = _vTargetPos;
	Vector3 vPosition = _vPos;
	Vector3 vDirVec = vTargetPos - vPosition;
	vDirVec.Normalized();
	vDirVec.y *= -1;
	float fTargetAngle = CMyMath::VectorToDegree(vDirVec);

	return fTargetAngle;
}

void CMissile::SetMissileType(E_MissileType _eMissileType)
{
	m_eMissileType = _eMissileType;
	switch (m_eMissileType) {
	case E_MissileType::GUIDED:
		m_fLivingMaxTime = 99.0f;
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
		SetPosition(vPosition.x + m_vPrevHeadDir.x * m_fSpeed * DeltaTime, vPosition.y - m_vPrevHeadDir.y * m_fSpeed * DeltaTime);
	}
		break;
	case E_MissileType::GUIDED:
	{
		if (nullptr == m_pTargetObject)
			break;

		// Cross dot
		Vector3 vTargetPos = m_pTargetObject->GetPosition(); // 적 위치

		Vector3 vTargetDir = vTargetPos - GetPosition(); // 방향벡터
		vTargetDir.Normalized();
		vTargetDir.y *= -1;
		Vector3 vHeadDir = m_vPrevHeadDir;
		vHeadDir.Normalized();

		// 현재 향하고있는 방향과 목표의 방향을 Cross한다.
		Vector3 vCross = CMyMath::GetCross(vTargetDir, vHeadDir);
		if (vCross.z < 0 )
			vHeadDir = Rotate(vHeadDir, 120.0f * DeltaTime);
		else if (vCross.z > 0)
			vHeadDir = Rotate(vHeadDir, -120.0f * DeltaTime);

		m_vPrevHeadDir = vHeadDir;
		SetPosition(GetPosition().x + vHeadDir.x * m_fSpeed * DeltaTime,  GetPosition().y - vHeadDir.y * m_fSpeed * DeltaTime);
	}
		break;
	case E_MissileType::UPGRADE:

		break;
	}
}
