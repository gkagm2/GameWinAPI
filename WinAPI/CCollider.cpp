#include "stdafx.h"
#include "CCollider.h"
#include "CCollisionManager.h"
#include "CObject.h"
#include "CEventManager.h"

UINT CCollider::g_iColliderID = 0;

CCollider::CCollider(CObject* _ownerObject) :
	m_pOwnerObject(_ownerObject),
	m_iCollisionCount(0),
	m_vOffsetPosition{},
	m_bIsActive(true),
	m_iID(g_iColliderID++),
	m_bIsTrigger(false)
{
	_ownerObject->SetCollider(this);
}

CCollider::CCollider(const CCollider& _other) :
	m_pOwnerObject(nullptr),
	m_iCollisionCount(0),
	m_vOffsetPosition{_other.m_vOffsetPosition},
	m_bIsActive(_other.m_bIsActive),
	m_iID(g_iColliderID++),
	m_bIsTrigger(_other.m_bIsTrigger)
{
}

CCollider::~CCollider()
{
}

void CCollider::OnCollisionStay(CCollider* _pOther)
{
	m_pOwnerObject->OnCollisionStay(_pOther->GetOwnerObject());
}

Vector3 CCollider::GetPosition()
{
	return GetOffsetPosition() + m_pOwnerObject->GetPosition();
}

void CCollider::SetActive(bool _bIsActive)
{
	m_bIsActive = _bIsActive;
	/*tEvent evt;
	evt.eType = E_EventType::COLLIDER_ACTIVE;
	evt.lParam = (DWORD_PTR)this;
	evt.wParam = (DWORD_PTR)_bIsActive;
	CEventManager::GetInstance()->AddEvent(evt);*/
}