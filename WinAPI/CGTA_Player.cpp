#include "stdafx.h"
#include "CResourceManager.h"
#include "CGTA_Player.h"
#include "CColliderRect.h"
#include "CTexture.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CKeyManager.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CCamera.h"
#include "CGTA_Character.h"
#include "CGTA_Citizen.h"
#include "CGTA_Bullet.h"
#include "CRigidbody2D.h"
#include "CGTA_Item.h"
#include "CGTA_AI.h"
#include "CDebug.h"


CGTA_Player::CGTA_Player(E_GroupType _eGroupType) :
	CGTA_Character(_eGroupType)
{
}

CGTA_Player::~CGTA_Player()
{
}

void CGTA_Player::Init()
{
	// Texture  set
	CTexture* pTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_gta_player);
	if (nullptr == pTexture)
		pTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_gta_player, STR_FILE_PATH_gta_player);
	SetTexture(pTexture);


	// Animator set
	SetAnimator(new CAnimator(this));
	GetAnimator()->CreateAnimation(L"idle", pTexture, Vector2(0, 40 * 0), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"idle_gun", pTexture, Vector2(0,40 * 1), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"stun", pTexture, Vector2(0,40 * 2), Vector2(40, 40), 1, 0.3f);

	GetAnimator()->CreateAnimation(L"dead1", pTexture, Vector2(0, 40 * 3), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"dead2", pTexture, Vector2(0, 40 * 4), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"dead3", pTexture, Vector2(0, 40 * 5), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"dead4", pTexture, Vector2(0, 40 * 6), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"dead5", pTexture, Vector2(0, 40 * 7), Vector2(40, 40), 1, 0.3f);

	GetAnimator()->CreateAnimation(L"creash", pTexture, Vector2(0,40 * 8), Vector2(40, 40), 2, 0.07f);
	GetAnimator()->CreateAnimation(L"run_gun", pTexture, Vector2(0,40 * 9), Vector2(40, 40), 8, 0.07f);
	GetAnimator()->CreateAnimation(L"walk_gun", pTexture, Vector2(0,40 * 10), Vector2(40, 40), 3, 0.07f);
	GetAnimator()->CreateAnimation(L"punch", pTexture, Vector2(0,40 * 11), Vector2(40, 40), 6, 0.07f);
	GetAnimator()->CreateAnimation(L"run", pTexture, Vector2(0,40 * 12), Vector2(40, 40), 8, 0.07f);
	GetAnimator()->CreateAnimation(L"walk", pTexture, Vector2(0, 40 * 13), Vector2(40, 40), 4, 0.07f);

	GetAnimator()->PlayAnimation(L"idle", E_AnimationPlayType::ONCE);

	float fAnimTextureWidth = GetAnimator()->GetAnimTexWidth();
	float fAnimTextureHeight = GetAnimator()->GetAnimTexHeight();
	Vector2 vScale{ fAnimTextureWidth, fAnimTextureHeight };
	SetScale(Vector3(vScale.x, vScale.y, 0.0f));

	SetPosition(0, 0, 0);

	SetObjectName(L"Player");
	CGTA_Character::Init();

	GetRigidbody()->SetMass(10.f);
}

void CGTA_Player::PrevUpdate()
{
	__super::PrevUpdate();
}

void CGTA_Player::Update()
{
	if (m_bIsDrive)
		Drive();
	else
		Move();

	m_fAttackCoolTime += DeltaTime;
	if (InputKeyHold(E_Key::Ctrl)) {
		if (m_fAttackCoolTime >= m_fAttackMaxCoolTime) {
			Attack();
			ActivePunchDetector(true);
			m_fAttackCoolTime = 0.f;
		}
	}
	if (InputKeyRelease(E_Key::Ctrl)) {
		if (E_WeaponType::FIST == GetCurWeaponType()) {

		}
	}

	if (InputKeyPress(E_Key::Z)) {
		ChangePrevWeapon();
	}
	if (InputKeyPress(E_Key::X)) {
		ChangeNextWeapon();
	}

	
	Debug->Print(Vector2(30, 80), L"s", GetWeaponInfo(GetCurWeaponType()).strName.c_str());
	State();
}

void CGTA_Player::LateUpdate()
{
	__super::LateUpdate();
}

void CGTA_Player::Render(HDC _hDC)
{
	CGTA_Character::Render(_hDC);
}

void CGTA_Player::OnCollisionEnter(CObject* _pOther)
{
}

void CGTA_Player::OnCollisionStay(CObject* _pOther)
{
}

void CGTA_Player::OnCollisionExit(CObject* _pOther)
{
}

void CGTA_Player::State()
{
	// State
	switch (m_eCharacterState) {
	case E_CharacterState::idle: {
		if (HaveGun())
			GetAnimator()->PlayAnimation(L"idle_gun", E_AnimationPlayType::LOOP);
		else
			GetAnimator()->PlayAnimation(L"idle", E_AnimationPlayType::LOOP);
	}
		break;
	case E_CharacterState::run:
		if (HaveGun())
			GetAnimator()->PlayAnimation(L"run_gun", E_AnimationPlayType::LOOP);
		else
			GetAnimator()->PlayAnimation(L"run", E_AnimationPlayType::LOOP);
		break;
	case E_CharacterState::walk:
		break;
	case E_CharacterState::attack:
		if (false == HaveGun()) {
			if (true == GetAnimator()->GetAnimation(L"punch")->IsFinish()) {
				if (m_bIsMoved)
					SetCharacterState(E_CharacterState::run);
				else
					SetCharacterState(E_CharacterState::idle);
			}
			else {
				GetAnimator()->PlayAnimation(L"punch", E_AnimationPlayType::ONCE);
				int iCurFrame = GetAnimator()->GetAnimation(L"punch")->GetCurFrame();
			}
		}
		break;
	case E_CharacterState::dead: {
		if(nullptr == GetAnimator()->GetCurAnimation())
			break;
		wstring strDeads[] = { L"dead1",L"dead2",L"dead3",L"dead4" };
		wstring strCurKeyName = GetAnimator()->GetCurAnimation()->GetName();
		bool bIsSetDeadAnim = false;
		for (int i = 0; i < 4; ++i) {
			if (0 == strDeads[i].compare(strCurKeyName)) {
				bIsSetDeadAnim = true;
				break;
			}
		}
		if (false == bIsSetDeadAnim) {
			int iRandom = rand() % 4;
			GetAnimator()->PlayAnimation(strDeads[iRandom], E_AnimationPlayType::ONCE);
		}
	}
		break;
	case E_CharacterState::getInTheCar:
		break;
	case E_CharacterState::getOffTheCar:
		break;
	case E_CharacterState::hit:
		break;
	}
}

void CGTA_Player::Move()
{
	Vector3 vHeadDir = GetUpVector();
	//RotateInfo().Update();
	if (InputKeyHold(E_Key::LEFT)) {
		RotateRP(-220 * DeltaTime);
	}
	if (InputKeyHold(E_Key::RIGHT)) {
		RotateRP(220 * DeltaTime);
	}

	if (InputKeyRelease(E_Key::UP)) {
		m_bIsMoved = false;
		SetCharacterState(E_CharacterState::idle);
	}
	if (InputKeyRelease(E_Key::DOWN)) {
		m_bIsMoved = false;
		SetCharacterState(E_CharacterState::idle);
	}

	if (InputKeyHold(E_Key::UP)) {
		m_bIsMoved = true;
		float x = GetPosition().x - GetUpVector().x * CharacterInfo().fMoveSpeed * DeltaTime;
		float y = GetPosition().y - GetUpVector().y * CharacterInfo().fMoveSpeed * DeltaTime;
		SetPosition(GetPosition().x - GetUpVector().x * CharacterInfo().fMoveSpeed * DeltaTime, GetPosition().y - GetUpVector().y * 300 * DeltaTime);
		SetCharacterState(E_CharacterState::run);
	}
	if (InputKeyHold(E_Key::DOWN)) {
		m_bIsMoved = true;
		SetPosition(GetPosition().x + GetUpVector().x * CharacterInfo().fMoveSpeed * DeltaTime, GetPosition().y + GetUpVector().y * CharacterInfo().fMoveSpeed * DeltaTime);
		SetCharacterState(E_CharacterState::run);
	}
}

void CGTA_Player::Attack()
{
	TWeaponInfo& tWeaponInfo = m_vecWeapon[(UINT)m_eCurWeaponType].second;
	E_WeaponType eWeaponType = GetCurWeaponType();

	// 총이면
	if (E_WeaponType::FIST != eWeaponType) {

		// 시민들 상태값 변환
		vector<CObject*>& vecObjs = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::CITIZEN);
		for (UINT i = 0; i < vecObjs.size(); ++i) {
			CGTA_Citizen* pCitizen = dynamic_cast<CGTA_Citizen*>(vecObjs[i]);
			if (pCitizen) {
				// 범위 내에 있는 사람들은 
				Vector3 vPosition = GetPosition();
				Vector3 vCitizenPos = pCitizen->GetPosition();
				float fDistance = (vPosition - vCitizenPos).GetDistance();

				if (fDistance <= m_fRunawayDistance) {
					if (pCitizen)
						pCitizen->Runaway();
				}
			}
		}
	}
	CGTA_Character::Attack();
}

void CGTA_Player::Drive()
{
}

void CGTA_Player::Dead()
{
}

void CGTA_Player::GetInTheVehicle()
{
}

void CGTA_Player::GetOutTheVehicle()
{
}