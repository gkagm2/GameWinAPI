#include "stdafx.h"
#include "CGTA_PunchDetector.h"
#include "CColliderRect.h"

CGTA_PunchDetector::CGTA_PunchDetector(E_GroupType _eGroupType) :
	CObject(_eGroupType)
{
	
}

CGTA_PunchDetector::~CGTA_PunchDetector()
{
}

void CGTA_PunchDetector::Init()
{
	CColliderRect* pCollider = new CColliderRect(this);
	SetCollider(pCollider);
	pCollider->SetScale(Vector3(10.f, 20.f, 0.f));
	pCollider->SetTrigger(true);
	CObject::Init();
}

void CGTA_PunchDetector::Update()
{
}

void CGTA_PunchDetector::OnCollisionEnter(CObject* _pOther)
{
	// ĳ���ͷ� ������Ʈ�� ��ȯ�Ѵ��� �˻��Ѵ�.
}

void CGTA_PunchDetector::OnCollisionStay(CObject* _pOther)
{
}

void CGTA_PunchDetector::OnCollisionExit(CObject* _pOther)
{
}