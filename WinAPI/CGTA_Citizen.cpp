#include "stdafx.h"
#include "CGTA_Citizen.h"
#include "CResourceManager.h"

#include "CAnimator.h"
#include "CAnimation.h"
#include "CObject.h"
#include "CGTA_Character.h"
#include "CColliderRect.h"
#include "CGTA_Player.h"
#include "CRigidbody2D.h"
#include "CGTA_AI.h"
#include "CPathFinding.h"
#include "CGTA_AIState.h"
#include "CGTA_IdleState.h"
#include "CGTA_RunawayState.h"
#include "CGTA_WanderState.h"


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
	pCollider->SetScale(Vector3(20.f, 20.f, 0.f));

	SetObjectName(L"Citizen");

	SetPosition(300,300, 0);
	CGTA_Character::Init();

	GetRigidbody()->SetMass(9.0f);

	// AI set
	CreateAI();
	CreatePathFinding();
	m_pPathFinding->AddObstacleTile(E_TileType::Wall);
	m_pPathFinding->AddObstacleTile(E_TileType::Water);

	GetAI()->AddState(L"idle", new CGTA_IdleState);
	GetAI()->AddState(L"runaway", new CGTA_RunawayState);
	GetAI()->AddState(L"wander", new CGTA_WanderState);
	GetAI()->ChangeState(L"wander");
	 

}

void CGTA_Citizen::Update()
{
	GetAI()->Update();
}

void CGTA_Citizen::LateUpdate()
{
	GetAI()->LateUpdate();
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

void CGTA_Citizen::State()
{
	switch (m_eCharacterState) {
	case E_CharacterState::idle:
		GetAnimator()->PlayAnimation(L"idle", E_AnimationPlayType::LOOP);
		break;
	case E_CharacterState::walk:
		GetAnimator()->PlayAnimation(L"walk", E_AnimationPlayType::LOOP);
		break;
	case E_CharacterState::run:
		GetAnimator()->PlayAnimation(L"run", E_AnimationPlayType::LOOP);
		break;
	case E_CharacterState::stun:
		GetAnimator()->PlayAnimation(L"stun", E_AnimationPlayType::LOOP);
		Stun();
		break;
	case E_CharacterState::dead:
		wstring strDeadArr[] = { L"dead1", L"dead2" };
		GetAnimator()->PlayAnimation(strDeadArr[rand() % 2].c_str(), E_AnimationPlayType::LOOP);
		Dead();
		break;
	}
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