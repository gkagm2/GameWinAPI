#include "stdafx.h"
#include "CRigidbody.h"
#include "CObject.h"
#include "CTimeManager.h"

// Test
#include "CCore.h"

CRigidbody::CRigidbody(CObject* _pTargetObj) :
	m_pOwnerObject(_pTargetObj),
	m_vForce{},
	m_vAccel{},
	m_fMass(0.f),
	m_vVelocity{},
	m_fDrag(0.f),
	m_fSpeed(0.f),
	m_fMaxSpeed(1000.0f),
	m_fFriction(0.f),
	m_bIsFrictionActive(true),
	m_vGracityAccel(0.0f, 9.80665f, 0.0f),
	m_bUseGravity(false),
	m_bIsKinematic(false)
{
	_pTargetObj->SetRigidbody(this);
}

CRigidbody::~CRigidbody()
{
}

void CRigidbody::PrevUpdate()
{
	m_vForce.Set(0.f,0.f,0.f);
}

void CRigidbody::Update()
{
	if (nullptr == m_pOwnerObject)
		return;
	if (m_bIsKinematic)
		return;
}


void CRigidbody::LateUpdate()
{
	if (nullptr == m_pOwnerObject)
		return;
	if (m_bIsKinematic)
		return;



	m_vAccel = m_vForce / m_fMass; // 가속도

		// 중력
	if (m_bUseGravity) { // f = m a = m 중력 가속도
		Vector3 vGravityAccel = m_vGracityAccel;
		vGravityAccel.y *= -1;
		m_vAccel -= vGravityAccel;
	}

	m_vVelocity += m_vAccel * DeltaTime;

	// 공기 저항
	Vector3 vDragDir = m_vVelocity;
	vDragDir.Normalized();
	Vector3 vDragVec = -vDragDir * m_fDrag * DeltaTime;
	float fDragSpeed = vDragVec.GetDistance();
	float fSpeed = m_vVelocity.GetDistance();
	if (fDragSpeed > fSpeed)
		m_vVelocity.Set(0.0f, 0.0f, 0.0f);
	else
		m_vVelocity += vDragVec;

	// 최대 속도 제한
	fSpeed = m_vVelocity.GetDistance();
	if (fSpeed > m_fMaxSpeed * DeltaTime) {
		m_vVelocity.Normalized();
		m_vVelocity = m_vVelocity * m_fMaxSpeed * DeltaTime;
	}

	Vector3 vObjPos = m_pOwnerObject->GetPosition();
	m_pOwnerObject->SetPosition(vObjPos + m_vVelocity);
}