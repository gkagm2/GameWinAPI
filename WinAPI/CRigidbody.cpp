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
	m_bIsKinematic(false),
	m_bIsActive(true)
{
	_pTargetObj->SetRigidbody(this);
}

CRigidbody::CRigidbody(const CRigidbody& _other) :
	m_pOwnerObject(nullptr),
	m_vForce{ _other.m_vForce },
	m_vAccel{ _other.m_vAccel},
	m_fMass( _other.m_fMass),
	m_vVelocity{ _other.m_vVelocity},
	m_fDrag( _other.m_fDrag),
	m_fSpeed( _other.m_fSpeed),
	m_fMaxSpeed( _other.m_fMaxSpeed),
	m_fFriction( _other.m_fFriction),
	m_bIsFrictionActive( _other.m_fFriction),
	m_vGracityAccel{ _other.m_vGracityAccel },
	m_bUseGravity( _other.m_bUseGravity),
	m_bIsKinematic( _other.m_bIsKinematic),
	m_bIsActive(_other.m_bIsActive)
{

}

CRigidbody::~CRigidbody()
{
}

void CRigidbody::PrevUpdate()
{
	if (false == m_bIsActive)
		return;
	m_vForce.Set(0.f,0.f,0.f);
}

void CRigidbody::Update()
{
	if (nullptr == m_pOwnerObject)
		return;
	if (m_bIsKinematic)
		return;
	if (false == m_bIsActive)
		return;
}

void CRigidbody::LateUpdate()
{
	if (false == m_bIsActive)
		return;
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