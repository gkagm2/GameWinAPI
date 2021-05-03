#include "stdafx.h"
#include "CResourceManager.h"
#include "CGTA_Player.h"
#include "CColliderRect.h"
#include "CTexture.h"
#include "CSound.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CKeyManager.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CCamera.h"
#include "CGTA_Character.h"
#include "CGTA_Citizen.h"
#include "CGTA_Cop.h"
#include "CGTA_Bullet.h"
#include "CRigidbody2D.h"
#include "CGTA_Item.h"
#include "CGTA_AI.h"
#include "CPathFinding.h"
#include "CTileMap.h"
#include "CGTA_Vehicle.h"

#include "CGTA_AI.h"
#include "CGTA_WalkToVehicleState.h"

#include "CGTA_UIContainer.h"
#include "CGTA_PlayerUI.h"
#include "CGTA_SuspectSearchSystem.h"
#include "CGTA_PunchDetector.h"

#include "CDebug.h"


CGTA_Player::CGTA_Player(E_GroupType _eGroupType) :
	CGTA_Character(_eGroupType),
	m_bIsActiveAI(false),
	m_iMoney(0)
{
}

CGTA_Player::~CGTA_Player()
{
}

void CGTA_Player::Init()
{
	// Texture  set
	CTexture* pTexture = CResourceManager::GetInstance()->GetTexture(STR_FILE_NAME_gta_player, STR_FILE_PATH_gta_player);
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

	SetObjectName(STR_OBJECT_NAME_Player);
	CGTA_Character::Init();

	GetRigidbody()->SetMass(10.f);
	
	// Test
	CharacterInfo().fHp = 100.f;

	// AI Init
	InitAI();

	SetCharacterState(E_CharacterState::idle);
}

void CGTA_Player::PrevUpdate()
{
	__super::PrevUpdate();
}

void CGTA_Player::Update()
{
	m_fAttackCoolTime += DeltaTime;
	
	if (GetCharacterState() != E_CharacterState::dead) {
		if (m_bDrive) {
			DriveUpdate();
			if (InputKeyPress(E_Key::Enter))
				GetOutTheVehicle();
		}
		else {
			MoveUpdate();
			if (InputKeyPress(E_Key::Enter))
				GetInTheVehicle();
			
			// Gun Fire
			if (InputKeyHold(E_Key::Ctrl)) {
				if (E_WeaponType::FIST == GetCurWeaponType())
					Attack();
				else {
					if (m_fAttackCoolTime >= m_fAttackMaxCoolTime) {
						Attack();
						m_fAttackCoolTime = 0.f;
					}
				}
			}
		}

		if (m_bIsActiveAI)
			GetAI()->Update();
	}

	if (InputKeyPress(E_Key::Z)) {
		ChangePrevWeapon();
	}
	if (InputKeyPress(E_Key::X)) {
		ChangeNextWeapon();
	}
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
	CGTA_Character::OnCollisionEnter(_pOther);
}

void CGTA_Player::OnCollisionStay(CObject* _pOther)
{
	CGTA_Character::OnCollisionStay(_pOther);
}

void CGTA_Player::OnCollisionExit(CObject* _pOther)
{
	CGTA_Character::OnCollisionExit(_pOther);
}

void CGTA_Player::DriveUpdate()
{
	SetRender(false);
	m_pVehicle->DriveUpdate();
}

void CGTA_Player::State()
{
	// State
	switch (m_eCharacterState) {
	case E_CharacterState::idle: {
		m_bIsMoved = false;
		if (HaveGun())
			GetAnimator()->PlayAnimation(L"idle_gun", E_AnimationPlayType::LOOP);
		else
			GetAnimator()->PlayAnimation(L"idle", E_AnimationPlayType::LOOP);
	}
		break;
	case E_CharacterState::run:
		m_bIsMoved = true;
		if (HaveGun())
			GetAnimator()->PlayAnimation(L"run_gun", E_AnimationPlayType::LOOP);
		else {
			wstring name = GetAnimator()->GetCurAnimation()->GetName();
			if (0 == GetAnimator()->GetCurAnimation()->GetName().compare(L"punch")) {
				if (true == GetAnimator()->GetAnimation(L"punch")->IsFinish())
					GetAnimator()->PlayAnimation(L"run", E_AnimationPlayType::LOOP);
			}
			else
				GetAnimator()->PlayAnimation(L"run", E_AnimationPlayType::LOOP);
		}
		break;
	case E_CharacterState::walk:
		m_bIsMoved = true;
		break;
	case E_CharacterState::attack:
		if (false == HaveGun()) {
			if (true == GetAnimator()->GetAnimation(L"punch")->IsFinish()) {
				if (m_bIsMoved)
					SetCharacterState(E_CharacterState::run);
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
	case E_CharacterState::hit:
		break;
	case E_CharacterState::stun:
		GetAnimator()->PlayAnimation(L"stun", E_AnimationPlayType::ONCE);
		break;
	case E_CharacterState::getInTheVehicle:
		if (HaveGun()) {
			if (m_bIsMoved)
				GetAnimator()->PlayAnimation(L"run_gun", E_AnimationPlayType::LOOP);
			else
				GetAnimator()->PlayAnimation(L"idle_gun", E_AnimationPlayType::LOOP);
		}
		else {
			if (m_bIsMoved)
				GetAnimator()->PlayAnimation(L"run", E_AnimationPlayType::LOOP);
			else
				GetAnimator()->PlayAnimation(L"idle", E_AnimationPlayType::LOOP);
		}
			
		break;
	case E_CharacterState::getOutTheVehicle:
		break;
	case E_CharacterState::drive:
		break;
	}

	if(m_bIsMoved)
		PlayFootStepSound();
}

void CGTA_Player::MoveUpdate()
{
	//RotateInfo().Update();
	if (InputKeyHold(E_Key::LEFT)) {
		RotateRP(-220 * DeltaTime);
		m_bIsActiveAI = false;
		m_bIsMoved = false;
	}
	if (InputKeyHold(E_Key::RIGHT)) {
		RotateRP(220 * DeltaTime);
		m_bIsActiveAI = false;
		m_bIsMoved = false;
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
		m_bIsActiveAI = false;
	}
	if (InputKeyHold(E_Key::DOWN)) {
		m_bIsMoved = true;
		SetPosition(GetPosition().x + GetUpVector().x * CharacterInfo().fMoveSpeed * DeltaTime, GetPosition().y + GetUpVector().y * CharacterInfo().fMoveSpeed * DeltaTime);
		SetCharacterState(E_CharacterState::run);
		m_bIsActiveAI = false;
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
			Vector3 vPosition = GetPosition();
			Vector3 vTargetPos = vecObjs[i]->GetPosition();
			float fDistance = (vPosition - vTargetPos).GetDistance();

			if (fDistance <= m_fNoticeDistance) {
				CGTA_Citizen* pCitizen = dynamic_cast<CGTA_Citizen*>(vecObjs[i]);
				if (pCitizen) {
					if (E_AIState::dead != pCitizen->GetCurAIState())
						if (E_AIState::runAway != pCitizen->GetAIState()) {
							wstring strHasGunSoundPath = Sound_HasGotaGunSound + std::to_wstring((rand() % Sound_HasGotaGunSound_Len) + 1);
							CSound* pSound = CResourceManager::GetInstance()->GetSound(strHasGunSoundPath, strHasGunSoundPath);
							pSound->Play();
							pCitizen->Runaway();
						}
					continue;
				}
				CGTA_Cop* pCop = dynamic_cast<CGTA_Cop*>(vecObjs[i]);
				if (pCop) {
					if (E_AIState::dead != pCop->GetCurAIState()) {
						if (E_AIState::trace != pCop->GetAIState()) {
							int iRandom = (rand() % Sound_Freeze_Len) + 1;
							wstring strPath = Sound_Freeze + std::to_wstring(iRandom);
							CSound* pFreezeSoun = CResourceManager::GetInstance()->GetSound(strPath, strPath);
							pFreezeSoun->Play();
							pCop->Trace();
						}
					}
					continue;
				}
			}
		}
	}

	// 주먹이면
	if (E_WeaponType::FIST == eWeaponType) {
		bool bDoPunch = false;
		wstring name = GetAnimator()->GetCurAnimation()->GetName();
		if (L"punch" != GetAnimator()->GetCurAnimation()->GetName()) {
			GetAnimator()->PlayAnimation(L"punch", E_AnimationPlayType::ONCE);
			GetAnimator()->GetCurAnimation()->Reset();
			bDoPunch = true;
		}
		else { // already set punch
			if (GetAnimator()->GetCurAnimation()->IsFinish()) { // end of animation
				GetAnimator()->GetCurAnimation()->Reset();
				bDoPunch = true;
			}
		}
		if (bDoPunch) {
			// Punch 컬라이더가 생성된다
			CGTA_PunchDetector* pPunchDetector = new CGTA_PunchDetector(E_GroupType::PUNCH, this);
			pPunchDetector->Init();
			pPunchDetector->SetRotateDegree(GetRotateDegree());
			pPunchDetector->SetPosition(GetPosition());
			CreateObject(pPunchDetector);
		}
	}
	else {
		// 총 타입에 따라 Shoot.
		// 총알 오브젝트 생성.
		CGTA_Bullet* pBullet = new CGTA_Bullet(E_GroupType::PROJECTILE, this);
		pBullet->Init();
		pBullet->SetDamage(tWeaponInfo.fDamage);

		pBullet->SetPosition(GetPosition() - GetNozzlePosition()); // 노즐 위치로 옮긴다.
		CreateObject(pBullet);

		bool bFindObj = false; // 시야각 내에 있는지 여부
		vector<CObject*>& vecObj = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::CITIZEN);

		Vector3 forwardVec = -(GetUpVector()); // 총을쏘는 캐릭터가 바라보고 있는 방향을 구한다.
		float fFieldOfViewAngle = 20.f; // 시야각
		float fMinDistance = 600.f;

		Vector3 vNearestObjPos;
		for (UINT i = 0; i < vecObj.size(); ++i) {
			CGTA_Citizen* pCitizen = (CGTA_Citizen*)vecObj[i];
			if (E_CharacterState::dead == pCitizen->GetCharacterState())
				continue;
			// 시야각 안에 들어오는지 확인한다.
			Vector3 vDirToTarget = vecObj[i]->GetPosition() - GetPosition();
			vDirToTarget.Normalized();
			float fDot = CMyMath::GetDot(-forwardVec, -vDirToTarget);
			float fAngle = acosf(fDot) * CMyMath::Rad2Deg();
			if (fAngle < fFieldOfViewAngle * 0.5f) { // 시야각 내에 있다는 것임
				float fDistance = CMyMath::GetDistance(GetPosition(), vecObj[i]->GetPosition());
				if (fDistance <= fMinDistance) {// 가장 가까이에 있는 오브젝트의 위치를 얻는다.
					bFindObj = true;
					fMinDistance = fDistance;
					vNearestObjPos = vecObj[i]->GetPosition();
				}
			}
		}

		if (bFindObj) { // 시야각 내에 오브젝트가 있으면
			pBullet->LookAt(vNearestObjPos, 400.f);
			float fDegree = pBullet->GetRotateDegree();
			pBullet->SetRotateDegree(fDegree - 180); // 방향 설정
		}
		else {
			float fDegree = GetRotateDegree();
			pBullet->SetRotateDegree(fDegree - 180); // 방향 설정
		}

		if (false == tWeaponInfo.bIsInfinite) { // 무한이 아닐 경우
			--tWeaponInfo.iBulletCnt;
			if (tWeaponInfo.iBulletCnt <= 0) {
				SetWeaponState(false, GetCurWeaponType());
				ChangeNextWeapon();
				return;
			}
		}
		CSound* pSound = CResourceManager::GetInstance()->GetSound(tWeaponInfo.strBulletSound, tWeaponInfo.strBulletSound);
		pSound->Stop(true);
		pSound->Play();
	}
	SetCharacterState(E_CharacterState::attack);
}

void CGTA_Player::Dead()
{
	CGTA_SuspectSearchSystem* pSusSystem = (CGTA_SuspectSearchSystem*)CSceneManager::GetInstance()->GetCurScene()->FindObject(STR_OBJECT_NAME_SuspectSearchSystem, E_GroupType::MANAGER);
	assert(pSusSystem);
	pSusSystem->ResetKiilCnt();

	GetCollider()->SetActive(false);
	CGTA_Character::Dead();
}

void CGTA_Player::Drive()
{
	SetActiveAI(false);
	MainCamera->SetTargetObject((CObject*)m_pVehicle);
	CGTA_Character::Drive();
}

void CGTA_Player::GetInTheVehicle()
{
	// 영역 안에 자동차가 있는지 확인한다.
	m_pVehicle = FindNearbyVehicle();
	if (nullptr == m_pVehicle)
		return;

	SetActiveAI(true);
	GetAI()->ChangeState(L"walkToVehicle");
	CGTA_Character::GetInTheVehicle();
}

void CGTA_Player::GetOutTheVehicle()
{
	SetRotateDegree(m_pVehicle->GetRotateDegree());
	MainCamera->SetTargetObject((CObject*)this);
	CGTA_Character::GetOutTheVehicle();
}

void CGTA_Player::HitByFist()
{
	CharacterInfo().fHp -= 1.f;
	CharacterInfo().fHp = max(CharacterInfo().fHp, 0);
	if (CharacterInfo().fHp <= 0)
		Dead();
}

void CGTA_Player::InitAI()
{
	CreateAI();
	CreatePathFinding();
	m_pPathFinding->AddObstacleTile(E_TileType::Wall);
	m_pPathFinding->AddObstacleTile(E_TileType::Water);

	GetAI()->AddState(L"walkToVehicle", new CGTA_WalkToVehicleState);
	Wander();
}

void CGTA_Player::ChangePrevWeapon()
{
	CGTA_Character::ChangePrevWeapon();
	// UI Set
	CGTA_UIContainer* pUIContainer = (CGTA_UIContainer*)CSceneManager::GetInstance()->GetCurScene()->FindObject(STR_OBJECT_NAME_UIContainer);
	assert(pUIContainer);
	pUIContainer->GetPlayerUI()->ChangeWeaponUI(GetWeaponInfo(GetCurWeaponType()));
}

void CGTA_Player::ChangeNextWeapon()
{
	CGTA_Character::ChangeNextWeapon();
	// UI Set
	CGTA_UIContainer* pUIContainer = (CGTA_UIContainer*)CSceneManager::GetInstance()->GetCurScene()->FindObject(STR_OBJECT_NAME_UIContainer);
	assert(pUIContainer);
	pUIContainer->GetPlayerUI()->ChangeWeaponUI(GetWeaponInfo(GetCurWeaponType()));
}