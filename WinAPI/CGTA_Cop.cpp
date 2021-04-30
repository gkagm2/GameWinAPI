#include "stdafx.h"
#include "CGTA_Cop.h"
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
#include "CGTA_Item.h"
#include "CGTA_Bullet.h"
CGTA_Cop::CGTA_Cop(E_GroupType _eGroupType) :
	CGTA_Character(_eGroupType)
{
}

CGTA_Cop::~CGTA_Cop()
{
}

void CGTA_Cop::Init()
{
	CTexture* pTexture = CResourceManager::GetInstance()->GetTexture(STR_FILE_PATH_gta_cops, STR_FILE_PATH_gta_cops);
	SetTexture(pTexture);

	// Animator set
	SetAnimator(new CAnimator(this));
	GetAnimator()->CreateAnimation(L"idle", pTexture, Vector2(0, 40 * 0), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"idle_gun", pTexture, Vector2(0, 40 * 1), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"stun", pTexture, Vector2(0, 40 * 2), Vector2(40, 40), 1, 0.3f);

	GetAnimator()->CreateAnimation(L"dead1", pTexture, Vector2(0, 40 * 3), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"dead2", pTexture, Vector2(0, 40 * 4), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"dead3", pTexture, Vector2(0, 40 * 5), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"dead4", pTexture, Vector2(0, 40 * 6), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"dead5", pTexture, Vector2(0, 40 * 7), Vector2(40, 40), 1, 0.3f);

	GetAnimator()->CreateAnimation(L"creash", pTexture, Vector2(0, 40 * 8), Vector2(40, 40), 2, 0.07f);
	GetAnimator()->CreateAnimation(L"run_gun", pTexture, Vector2(0, 40 * 9), Vector2(40, 40), 8, 0.07f);
	GetAnimator()->CreateAnimation(L"walk_gun", pTexture, Vector2(0, 40 * 10), Vector2(40, 40), 3, 0.25f);
	GetAnimator()->CreateAnimation(L"punch", pTexture, Vector2(0, 40 * 11), Vector2(40, 40), 6, 0.07f);
	GetAnimator()->CreateAnimation(L"run", pTexture, Vector2(0, 40 * 12), Vector2(40, 40), 8, 0.07f);
	GetAnimator()->CreateAnimation(L"walk", pTexture, Vector2(0, 40 * 13), Vector2(40, 40), 4, 0.25f);

	GetAnimator()->PlayAnimation(L"idle", E_AnimationPlayType::ONCE);

	float fAnimTextureWidth = GetAnimator()->GetAnimTexWidth();
	float fAnimTextureHeight = GetAnimator()->GetAnimTexHeight();
	Vector2 vScale{ fAnimTextureWidth, fAnimTextureHeight };
	SetScale(Vector3(vScale.x, vScale.y, 0.0f));

	SetPosition(0, 0, 0);

	CharacterInfo().fMoveSpeed *= 0.7f;

	SetObjectName(L"Cop");
	CGTA_Character::Init();
	GetRigidbody()->SetMass(9.0f);

	// AI set
	InitAI();
}

void CGTA_Cop::PrevUpdate()
{
	__super::PrevUpdate();
}

void CGTA_Cop::Update()
{
	GetAI()->Update();

	State();
}

void CGTA_Cop::LateUpdate()
{
	GetAI()->LateUpdate();
	__super::LateUpdate();
}

void CGTA_Cop::Render(HDC _hDC)
{
	CGTA_Character::Render(_hDC);
}

void CGTA_Cop::OnCollisionEnter(CObject* _pOther)
{
	CGTA_Character::OnCollisionEnter(_pOther);
}

void CGTA_Cop::OnCollisionStay(CObject* _pOther)
{
	CGTA_Character::OnCollisionStay(_pOther);
}

void CGTA_Cop::OnCollisionExit(CObject* _pOther)
{
	CGTA_Character::OnCollisionExit(_pOther);
}

void CGTA_Cop::State()
{
	// State
	switch (m_eCharacterState) {
	case E_CharacterState::idle: {
		m_bIsMoved = false;
		if (HaveGun())
			GetAnimator()->PlayAnimation(L"idle_gun", E_AnimationPlayType::LOOP);
		else
			GetAnimator()->PlayAnimation(L"idle", E_AnimationPlayType::LOOP);
		break;
	}
	case E_CharacterState::run:
		m_bIsMoved = true;
		if (HaveGun())
			GetAnimator()->PlayAnimation(L"run_gun", E_AnimationPlayType::LOOP);
		else
			GetAnimator()->PlayAnimation(L"run", E_AnimationPlayType::LOOP);
		break;
	case E_CharacterState::walk:
		m_bIsMoved = true;
		if (HaveGun())
			GetAnimator()->PlayAnimation(L"walk_gun", E_AnimationPlayType::LOOP);
		else
			GetAnimator()->PlayAnimation(L"walk", E_AnimationPlayType::LOOP);
		break;
	case E_CharacterState::attack:
		if (false == HaveGun()) {
			if (true == GetAnimator()->GetAnimation(L"punch")->IsFinish()) {
				if (m_bIsMoved)
					SetCharacterState(E_CharacterState::walk);
				else
					SetCharacterState(E_CharacterState::idle);
			}
		}
		else {
			if (m_bIsMoved)
				SetCharacterState(E_CharacterState::run);
			else
				SetCharacterState(E_CharacterState::idle);
		}
		break;
	case E_CharacterState::dead: {
		if (nullptr == GetAnimator()->GetCurAnimation())
			break;
		wstring strDeads[] = { L"dead1",L"dead2",L"dead3",L"dead4" };
		wstring strCurKeyName = GetAnimator()->GetCurAnimation()->GetName();
		bool bIsSetDeadAnim = false;
		for (int i = 0; i < 4; ++i) {
			if (strDeads[i].compare(strCurKeyName) == 0) {
				bIsSetDeadAnim = true;
				break;
			}
		}
		if (false == bIsSetDeadAnim) {
			int iRandom = rand() % 4;
			GetAnimator()->PlayAnimation(strDeads[iRandom], E_AnimationPlayType::ONCE);
		}
		break;
	}
	case E_CharacterState::hit:
		break;
	case E_CharacterState::stun:
		GetAnimator()->PlayAnimation(L"stun", E_AnimationPlayType::ONCE);
		break;
	case E_CharacterState::getInTheVehicle:
		if (HaveGun())
			GetAnimator()->PlayAnimation(L"run_gun", E_AnimationPlayType::LOOP);
		else
			GetAnimator()->PlayAnimation(L"run", E_AnimationPlayType::LOOP);
		break;
	case E_CharacterState::getOutTheVehicle:
		break;
	case E_CharacterState::drive:
		break;
	}
}

void CGTA_Cop::Drive()
{
	GetAI()->ChangeState(L"drive");
	CGTA_Character::Drive();
}

void CGTA_Cop::HitByFist()
{
	GetAI()->ChangeState(L"stun");
	CGTA_Character::HitByFist();
}

void CGTA_Cop::Dead()
{
	DropWeaponItem();
	
	GetAI()->ChangeState(L"dead");
	CGTA_Character::Dead();
}

void CGTA_Cop::GetInTheVehicle()
{
	GetAI()->ChangeState(L"walkToVehicle");
	CGTA_Character::GetInTheVehicle();
}

void CGTA_Cop::GetOutTheVehicle()
{
	CGTA_Character::GetOutTheVehicle();
}

void CGTA_Cop::Wander()
{
	GetAI()->ChangeState(L"wander");
	CGTA_Character::Wander();
}

void CGTA_Cop::Trace()
{
	GetAI()->ChangeState(L"trace");
	CGTA_Character::Trace();
}