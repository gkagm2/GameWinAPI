#include "stdafx.h"
#include "CCollider.h"
#include "CCollisionManager.h"
#include "CObject.h"

UINT CCollider::g_iColliderID = 0;

CCollider::CCollider(CObject* _ownerObject) :
	m_pOwnerObject(_ownerObject),
	m_iCollisionCount(0),
	m_vOffsetPosition{},
	m_bIsActive(true),
	m_iID(g_iColliderID++)
{
	_ownerObject->SetCollider(this);
	CCollisionManager::GetInstance()->AddCollision(this);
}

CCollider::~CCollider()
{
}

Vector3 CCollider::GetPosition()
{
	return GetOffsetPosition() + m_pOwnerObject->GetPosition();
}