#include "stdafx.h"
#include "CGTA_PunchDetector.h"
#include "CColliderRect.h"
#include "CGTA_Character.h"
#include "CTimeManager.h"

CGTA_PunchDetector::CGTA_PunchDetector(E_GroupType _eGroupType, CObject* _OwnerObj) :
	CObject(_eGroupType),
	m_pOwnerObj(_OwnerObj),
	m_fPunchCoolTime(0.f),
	m_fPunchMaxCoolTime(0.3f),
	m_fFinalPos{},
	m_fOffsetPos{}
{
}

CGTA_PunchDetector::~CGTA_PunchDetector()
{
}

void CGTA_PunchDetector::Init()
{
	CColliderRect* pCollider = new CColliderRect(this);
	pCollider->SetOffsetPosition(Vector3(0, 0, 0));
	pCollider->SetScale(Vector3(30, 30, 0));
	pCollider->SetTrigger(true);

	CObject::Init();
}

void CGTA_PunchDetector::Update()
{
	if (nullptr == m_pOwnerObj)
		return;

	m_fOffsetPos = m_pOwnerObj->GetUpVector() * -1 * 20.f;

	m_fFinalPos = m_pOwnerObj->GetPosition() + m_fOffsetPos;
	SetPosition(m_fFinalPos);

	m_fPunchCoolTime += DeltaTime;
	if (m_fPunchCoolTime >= m_fPunchMaxCoolTime) {
		DestroyObject(this);
		m_fPunchCoolTime = 0.f;
	}
}

void CGTA_PunchDetector::Render(HDC _hDC)
{
	if (false == IsRender())
		return;

	if (nullptr != GetCollider()) {
		if (GetCollider()->IsRender())
			GetCollider()->Render(_hDC);
	}
}

void CGTA_PunchDetector::OnCollisionEnter(CObject* _pOther)
{
	CGTA_Character* pCharacter = dynamic_cast<CGTA_Character*>(_pOther);
	if (pCharacter) {
		if (m_pOwnerObj != (CObject*)pCharacter) {
			pCharacter->HitByFist();
		}
	}
}