#include "stdafx.h"
#include "CGTA_Citizen.h"
#include "CResourceManager.h"

#include "CAnimator.h"
#include "CAnimation.h"

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
		pTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_gta_citizen, STR_FILE_NAME_gta_citizen);
	SetTexture(pTexture);

	// Animator set
	SetAnimator(new CAnimator(this));
	GetAnimator()->CreateAnimation(L"idle", pTexture, Vector2(0, 40 * 0), Vector2(40, 40), 1, 0.3f);
}

void CGTA_Citizen::PrevUpdate()
{
}

void CGTA_Citizen::Update()
{
}

void CGTA_Citizen::LateUpdate()
{
}

void CGTA_Citizen::Render(HDC _hDC)
{
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

void CGTA_Citizen::Shoot()
{
}

void CGTA_Citizen::Drive()
{
}

void CGTA_Citizen::Punch()
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
