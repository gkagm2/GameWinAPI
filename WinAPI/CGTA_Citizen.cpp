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
#include "CGTA_Bullet.h"


CGTA_Citizen::CGTA_Citizen(E_GroupType _eGroupType) :
	CGTA_Character(_eGroupType)
{
}

CGTA_Citizen::~CGTA_Citizen()
{
}

void CGTA_Citizen::Init()
{
	CTexture* pTexture = CResourceManager::GetInstance()->GetTexture(STR_FILE_NAME_gta_citizen, STR_FILE_PATH_gta_citizen);
	SetTexture(pTexture);

	// Animator set
	SetAnimator(new CAnimator(this));
	GetAnimator()->CreateAnimation(L"idle", pTexture, Vector2(0, 40 * 0), Vector2(40, 40), 2, 5.f);
	GetAnimator()->CreateAnimation(L"stun", pTexture, Vector2(0, 40 * 1), Vector2(40, 40), 1, 1.f);
	GetAnimator()->CreateAnimation(L"dead1", pTexture, Vector2(0, 40 * 2), Vector2(40, 40), 1, 1.f);
	GetAnimator()->CreateAnimation(L"dead2", pTexture, Vector2(0, 40 * 3), Vector2(40, 40), 1, 1.f);
	GetAnimator()->CreateAnimation(L"walk", pTexture, Vector2(0, 40 * 4), Vector2(40, 40), 4, 0.25f);
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

	CharacterInfo().fMoveSpeed *= 0.7f;

	// AI set
	InitAI();
}

void CGTA_Citizen::Update()
{
	GetAI()->Update();
	State();
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
	CGTA_Character::OnCollisionEnter(_pOther);
}

void CGTA_Citizen::OnCollisionStay(CObject* _pOther)
{
	CGTA_Character::OnCollisionStay(_pOther);
}

void CGTA_Citizen::OnCollisionExit(CObject* _pOther)
{
	CGTA_Character::OnCollisionExit(_pOther);
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
		break;
	case E_CharacterState::dead:
		wstring strDeadArr[] = { L"dead1", L"dead2" };
		bool bIsDeadAnim = false;
		for (int i = 0; i < 2; ++i) {
			if (0 == GetAnimator()->GetCurAnimation()->GetName().compare(strDeadArr[i])) {
				bIsDeadAnim = true;
				break;
			}
		}
		if (false == bIsDeadAnim) {
			GetAnimator()->PlayAnimation(strDeadArr[rand() % 2].c_str(), E_AnimationPlayType::LOOP);
		}
		break;
	}
}

void CGTA_Citizen::Attack()
{
	TWeaponInfo& tWeaponInfo = m_vecWeapon[(UINT)m_eCurWeaponType].second;
	E_WeaponType eWeaponType = GetCurWeaponType();

	// 주먹이면
	if (E_WeaponType::FIST == eWeaponType) {
		GetAnimator()->PlayAnimation(L"punch", E_AnimationPlayType::ONCE);
		// Punch 컬라이더가 생성된다
	}
	else {
		// 총 타입에 따라 Shoot.

		// 총알 오브젝트 생성.
		CGTA_Bullet* pBullet = new CGTA_Bullet(E_GroupType::PROJECTILE, this);
		pBullet->SetOwnerObj(this);
		pBullet->Init();
		float fDegree = GetRotateDegree();
		pBullet->SetRotateDegree(fDegree); // 방향 설정
		pBullet->RotateRP(180);
		pBullet->SetPosition(GetPosition() - GetNozzlePosition()); // 노즐 위치로 옮긴다.
		CreateObject(pBullet);

		if (false == tWeaponInfo.bIsInfinite) { // 무한이 아닐 경우
			--tWeaponInfo.iBulletCnt;
			if (tWeaponInfo.iBulletCnt <= 0) {
				SetWeaponState(false, GetCurWeaponType());
				ChangeNextWeapon();
				return;
			}
		}
	}
	SetCharacterState(E_CharacterState::attack);
}

void CGTA_Citizen::Drive()
{
	GetAI()->ChangeState(L"drive");
	CGTA_Character::Drive();
}

void CGTA_Citizen::GetInTheVehicle()
{
	GetAI()->ChangeState(L"walkToVehicle");
	CGTA_Character::GetInTheVehicle();
}

void CGTA_Citizen::GetOutTheVehicle()
{
	CGTA_Character::GetOutTheVehicle();
}

void CGTA_Citizen::HitByFist()
{
	GetAI()->ChangeState(L"stun");
	CGTA_Character::HitByFist();
}

void CGTA_Citizen::Dead()
{
	GetAI()->ChangeState(L"dead");
	CGTA_Character::Dead();
}

void CGTA_Citizen::Wander()
{
	GetAI()->ChangeState(L"wander");
	CGTA_Character::Wander();
}

void CGTA_Citizen::Runaway()
{
	GetAI()->ChangeState(L"runaway");
	CGTA_Character::Runaway();
}