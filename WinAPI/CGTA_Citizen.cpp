#include "stdafx.h"
#include "CGTA_Citizen.h"
#include "CResourceManager.h"

#include "CAnimator.h"
#include "CAnimation.h"
#include "CObject.h"
#include "CGTA_Character.h"
#include "CColliderRect.h"
#include "CGTA_Player.h"

CGTA_Citizen::CGTA_Citizen(E_GroupType _eGroupType) :
	CGTA_Character(_eGroupType)
{
}

CGTA_Citizen::~CGTA_Citizen()
{
}

void CGTA_Citizen::Init()
{
	CTexture* pTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_gta_citizen);
	if (nullptr == pTexture)
		pTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_gta_citizen, STR_FILE_PATH_gta_citizen);
	SetTexture(pTexture);

	// Animator set
	SetAnimator(new CAnimator(this));
	GetAnimator()->CreateAnimation(L"idle", pTexture, Vector2(0, 40 * 0), Vector2(40, 40), 2, 5.f);
	GetAnimator()->CreateAnimation(L"stun", pTexture, Vector2(0, 40 * 1), Vector2(40, 40), 1, 1.f);
	GetAnimator()->CreateAnimation(L"dead1", pTexture, Vector2(0, 40 * 2), Vector2(40, 40), 1, 1.f);
	GetAnimator()->CreateAnimation(L"dead2", pTexture, Vector2(0, 40 * 3), Vector2(40, 40), 1, 1.f);
	GetAnimator()->CreateAnimation(L"walk", pTexture, Vector2(0, 40 * 4), Vector2(40, 40), 4, 0.15f);
	GetAnimator()->CreateAnimation(L"run", pTexture, Vector2(0, 40 * 5), Vector2(40, 40), 10, 0.05f);

	GetAnimator()->PlayAnimation(L"walk", E_AnimationPlayType::LOOP);

	float fAnimTextureWidth = GetAnimator()->GetAnimTexWidth();
	float fAnimTextureHeight = GetAnimator()->GetAnimTexHeight();
	Vector2 vScale{ fAnimTextureWidth, fAnimTextureHeight };
	SetScale(Vector3(vScale.x, vScale.y, 0.f));

	// Collider set
	CColliderRect* pCollider = new CColliderRect(this);
	SetCollider(pCollider);
	pCollider->SetScale(Vector3(15.f, 15.f, 0.f));

	SetObjectName(L"Citizen");

	SetPosition(300,300, 0);
	CGTA_Character::Init();
}

void CGTA_Citizen::PrevUpdate()
{
	__super::PrevUpdate();
}

void CGTA_Citizen::Update()
{
}

void CGTA_Citizen::LateUpdate()
{
	__super::LateUpdate();
}

void CGTA_Citizen::Render(HDC _hDC)
{
	CGTA_Character::Render(_hDC);
}

void CGTA_Citizen::OnCollisionEnter(CObject* _pOther)
{
}

void CGTA_Citizen::OnCollisionStay(CObject* _pOther)
{
}

void CGTA_Citizen::OnCollisionExit(CObject* _pOther)
{
}

void CGTA_Citizen::Move()
{
}

void CGTA_Citizen::Attack()
{
}

void CGTA_Citizen::Drive()
{
}

void CGTA_Citizen::Dead()
{
}

void CGTA_Citizen::GetInTheVehicle()
{
}

void CGTA_Citizen::GetOutTheVehicle()
{
}