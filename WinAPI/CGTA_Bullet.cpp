#include "stdafx.h"
#include "CGTA_Bullet.h"
#include "CColliderRect.h"
#include "CTimeManager.h"
#include "CTexture.h"
#include "CResource.h"
#include "CResourceManager.h"

CGTA_Bullet::CGTA_Bullet(E_GroupType e_GroupType) :
	CObject(m_eGroupType),
	m_fLifeCoolTime(0.f),
	m_fLifeMaxCoolTime(7.f),
	m_fSpeed(140.f)
{

}

CGTA_Bullet::CGTA_Bullet(const CGTA_Bullet& _origin) :
	CObject(_origin),
	m_fLifeCoolTime(0.f),
	m_fLifeMaxCoolTime(_origin.m_fLifeMaxCoolTime),
	m_fSpeed(_origin.m_fSpeed)
{
}

CGTA_Bullet::~CGTA_Bullet()
{
}

void CGTA_Bullet::Init()
{
	// 텍스쳐 설정
	CTexture* pTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_PATH_gta_Bullet);
	if (nullptr == pTexture) {
		pTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_PATH_gta_Bullet, STR_FILE_PATH_gta_Bullet);
	}
	SetTexture(pTexture);

	// 충돌 설정
	CColliderRect* pCollider = new CColliderRect(this);
	SetCollider(pCollider);
	pCollider->SetScale(Vector3(10.f, 10.f, 0.f));

	// 크기 설정
	SetScale(Vector3(20.f, 20.f, 0.f));

	CObject::Init();
}

void CGTA_Bullet::Update()
{
	m_fLifeCoolTime += DeltaTime;
	if (m_fLifeCoolTime >= m_fLifeMaxCoolTime)
		DeleteObject(this);

	Vector3 vHeadDir = GetUpVector();
	SetPosition(GetPosition() +vHeadDir * DeltaTime * m_fSpeed);
}

void CGTA_Bullet::OnCollisionEnter(CObject* _pOther)
{
}

void CGTA_Bullet::OnCollisionStay(CObject* _pOther)
{
}

void CGTA_Bullet::OnCollisionExit(CObject* _pOther)
{
}