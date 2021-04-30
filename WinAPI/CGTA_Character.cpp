#include "stdafx.h"
#include "CGTA_Character.h"
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
#include "CCamera2D.h"
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CGTA_AI.h"
#include "CGTA_AIState.h"
#include "CPathFinding.h"
#include "CGTA_Bullet.h"
#include "CGTA_Item.h"
#include "CGTA_PunchDetector.h"
#include "CGTA_Citizen.h"
#include "CGTA_Cop.h"
#include "CGTA_Vehicle.h"

#include "CGTA_IdleState.h"
#include "CGTA_RunawayState.h"
#include "CGTA_WanderState.h"
#include "CGTA_TraceState.h"
#include "CGTA_DeadState.h"
#include "CGTA_StunState.h"
#include "CGTA_WalkToVehicleState.h"

#include "CSound.h"

// TEST
#include "CCore.h"
#include "CDebug.h"

CGTA_Character::CGTA_Character(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_tInfo{},
	m_bDrive(false),
	m_bIsMoved(false),
	m_fAttackCoolTime(0.f),
	m_fAttackMaxCoolTime(0.f),
	m_fStunCoolTime(0.f),
	m_fStunMaxCoolTime(10.f),
	m_fDeadCoolTime(0.f),
	m_fDeadMaxCoolTime(20.f),
	m_fNoticeDistance(500.f),
	m_fVehicleSearchDistance(600.f),
	m_pVehicle(nullptr),
	m_eCurWeaponType(E_WeaponType::FIST),
	m_eCharacterState(E_CharacterState::idle),
	m_eAIState(E_AIState::wander),
	m_pAI(nullptr),
	m_pPathFinding(nullptr),
	m_pNozzleTexture(nullptr),
	// Sound
	pHasGotaGunSound{ nullptr, },
	pHelpSound{ nullptr, },
	pOMGSound{ nullptr, },
	pScreamSound{ nullptr, },
	pFreezeSound{ nullptr, },
	pFootsepConcrete{ nullptr, }
{
	// weapon system
	int iSize = (int)E_WeaponType::END;
	m_vecWeapon.resize(iSize, std::make_pair(false, TWeaponInfo{}));
	m_vecWeapon[(UINT)E_WeaponType::FIST].second.strName = STR_NAME_Fist;
	m_vecWeapon[(UINT)E_WeaponType::FIST].first = true;
	m_vecWeapon[(UINT)E_WeaponType::FIST].second.bIsInfinite = true;
	m_vecWeapon[(UINT)E_WeaponType::FIST].second.fShootCoolTime = 0.7f;
	m_fAttackMaxCoolTime = m_vecWeapon[(UINT)E_WeaponType::FIST].second.fShootCoolTime;
	m_vecWeapon[(UINT)E_WeaponType::FIST].second.strSound = STR_FILE_PATH_GTA_Sound_PunchHit;
	InitSound();
}

CGTA_Character::CGTA_Character(const CGTA_Character& _origin) :
	CObject(_origin),
	m_tInfo(_origin.m_tInfo),
	m_bDrive(false),
	m_bIsMoved(false),
	m_fAttackCoolTime(0.f),
	m_fAttackMaxCoolTime(0.f),
	m_fStunCoolTime(0.f),
	m_fStunMaxCoolTime(10.f),
	m_fDeadCoolTime(0.f),
	m_fDeadMaxCoolTime(20.f),
	m_fNoticeDistance(_origin.m_fNoticeDistance),
	m_fVehicleSearchDistance(_origin.m_fVehicleSearchDistance),
	m_pVehicle(nullptr),
	m_eCurWeaponType(_origin.m_eCurWeaponType),
	m_eCharacterState(_origin.m_eCharacterState),
	m_eAIState(_origin.m_eAIState),
	m_pAI(nullptr),
	m_pPathFinding(nullptr),
	m_pNozzleTexture(_origin.m_pNozzleTexture),
	//Sound
	pHasGotaGunSound{ nullptr, },
	pHelpSound{ nullptr, },
	pOMGSound{ nullptr, },
	pScreamSound{ nullptr, },
	pFreezeSound{ nullptr, },
	pFootsepConcrete{ nullptr, }
{
	if (nullptr != _origin.m_pAI) {
		m_pAI = _origin.m_pAI->Clone();
		m_pAI->m_pCharacter = this;

		if (nullptr != _origin.m_pAI->GetCurState())
			m_pAI->ChangeState(_origin.m_pAI->GetCurState()->GetName());
	}
		
	if (nullptr != _origin.m_pPathFinding)
		m_pPathFinding = _origin.m_pPathFinding->Clone();
	m_vecWeapon.resize(_origin.m_vecWeapon.size());
	m_vecWeapon.assign(_origin.m_vecWeapon.begin(), _origin.m_vecWeapon.end());

	InitSound();
}

CGTA_Character::~CGTA_Character()
{
	if (nullptr != m_pAI)
		delete m_pAI;
	if (nullptr != m_pPathFinding)
		delete m_pPathFinding;
}
		
void CGTA_Character::InitSound()
{
	for (int i = 0; i < Sound_HasGotaGunSound_Len; ++i) {
		wstring strPath = Sound_HasGotaGunSound + std::to_wstring(i + 1) + Sound_WAV;
		pHasGotaGunSound[i] = CResourceManager::GetInstance()->FindSound(strPath);
		if (nullptr == pHasGotaGunSound[i]) {
			pHasGotaGunSound[i] = CResourceManager::GetInstance()->LoadSound(strPath, strPath);
			assert(pHasGotaGunSound[i]);
		}
	}

	for (int i = 0; i < Sound_Help_Len; ++i) {
		wstring strPath = Sound_Help + std::to_wstring(i+1) + Sound_WAV;
		pHelpSound[i] = CResourceManager::GetInstance()->FindSound(strPath);
		if (nullptr == pHelpSound[i]) {
			pHelpSound[i] = CResourceManager::GetInstance()->LoadSound(strPath, strPath);
			assert(pHelpSound[i]);
		}
	}

	for (int i = 0; i < Sound_OMG_Len; ++i) {
		wstring strPath = Sound_OMG + std::to_wstring(i + 1) + Sound_WAV;
		pOMGSound[i] = CResourceManager::GetInstance()->FindSound(strPath);
		if (nullptr == pOMGSound[i]) {
			pOMGSound[i] = CResourceManager::GetInstance()->LoadSound(strPath, strPath);
			assert(pOMGSound[i]);
		}
	}

	for (int i = 0; i < Sound_Scream_Len; ++i) {
		wstring strPath = Sound_Scream + std::to_wstring(i + 1) + Sound_WAV;
		pScreamSound[i] = CResourceManager::GetInstance()->FindSound(strPath);
		if (nullptr == pScreamSound[i]) {
			pScreamSound[i] = CResourceManager::GetInstance()->LoadSound(strPath, strPath);
			assert(pScreamSound[i]);
		}
	}
	
	for (int i = 0; i < Sound_Freeze_Len; ++i) {
		wstring strPath = Sound_Scream + std::to_wstring(i + 1) + Sound_WAV;
		pFreezeSound[i] = CResourceManager::GetInstance()->FindSound(strPath);
		if (nullptr == pFreezeSound[i]) {
			pFreezeSound[i] = CResourceManager::GetInstance()->LoadSound(strPath, strPath);
			assert(pFreezeSound[i]);
		}
	}

	for (int i = 0; i < Sound_Concreate_Len; ++i) {
		wstring strPath = Sound_Scream + std::to_wstring(i + 1) + Sound_WAV;
		pFootsepConcrete[i] = CResourceManager::GetInstance()->FindSound(strPath);
		if (nullptr == pFootsepConcrete[i]) {
			pFootsepConcrete[i] = CResourceManager::GetInstance()->LoadSound(strPath, strPath);
			assert(pFootsepConcrete[i]);
		}
	}
}

void CGTA_Character::Init()
{
	// Collider set
	CColliderRect* pCollider = new CColliderRect(this);
	pCollider->SetScale(Vector3(20.f, 20.f, 0.f));

	CRigidbody2D* pRigidbody = new CRigidbody2D(this);
	SetRigidbody(pRigidbody);

	CObject::Init();
}

void CGTA_Character::InitAI()
{
	CreateAI();
	CreatePathFinding();
	m_pPathFinding->AddObstacleTile(E_TileType::Wall);
	m_pPathFinding->AddObstacleTile(E_TileType::Water);

	GetAI()->AddState(L"idle", new CGTA_IdleState);
	GetAI()->AddState(L"runaway", new CGTA_RunawayState);
	GetAI()->AddState(L"wander", new CGTA_WanderState);
	GetAI()->AddState(L"trace", new CGTA_TraceState);
	GetAI()->AddState(L"dead", new CGTA_DeadState);
	GetAI()->AddState(L"stun", new CGTA_StunState);
	GetAI()->AddState(L"walkToVehicle", new CGTA_WalkToVehicleState);
	Wander();
}

void CGTA_Character::PrevUpdate()
{
	__super::PrevUpdate();
}

void CGTA_Character::Update()
{
	m_pAI->Update();
}

void CGTA_Character::LateUpdate()
{
	__super::LateUpdate();
}

void CGTA_Character::Render(HDC _hDC)
{
	if (false == IsRender())
		return;

	//
	bool bFindObj = false; // 시야각 내에 있는지 여부
	Vector3 forwardVec = -(GetUpVector()); // 총을쏘는 캐릭터가 바라보고 있는 방향을 구한다.
	Vector3 renderPos = MainCamera->GetRenderPosition(GetPosition());
	float fFieldOfViewAngle = 20.f; // 시야각

	Vector3 vLeftFieldOfViewDir = Rotate(forwardVec, -fFieldOfViewAngle * 0.5f);
	Vector3 vRightFieldOfViewDir = Rotate(forwardVec, fFieldOfViewAngle * 0.5f);

	MoveToEx(_hDC, (int)renderPos.x, (int)renderPos.y, nullptr);
	LineTo(_hDC, int(renderPos.x + vRightFieldOfViewDir.x * 400.f), int(renderPos.y + vRightFieldOfViewDir.y * 400.f));

	MoveToEx(_hDC, (int)renderPos.x, (int)renderPos.y, nullptr);
	LineTo(_hDC, int(renderPos.x + vLeftFieldOfViewDir.x * 400.f), int(renderPos.y + vLeftFieldOfViewDir.y * 400.f));
	//

	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	if (GetTexture()) {
		if (nullptr != GetAnimator())
			GetAnimator()->Render(_hDC);
		else {
			RenderRotatedTex(_hDC, vRenderPosition, GetTexture()->GetDC(), 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight());
		}
	}

	if (nullptr != GetCollider()) {
		if (GetCollider()->IsRender())
			GetCollider()->Render(_hDC);
	}
	
	if (m_pAI) {
		m_pAI->Render(_hDC);
	}
		
	//__super::Render(_hDC);
}

void CGTA_Character::OnCollisionEnter(CObject* _pOther)
{
	CGTA_Bullet* pBullet = dynamic_cast<CGTA_Bullet*>(_pOther);
	if (pBullet) {
		TCharacterInfo& tCharacterInfo = CharacterInfo();
		if (tCharacterInfo.fArmor > 0.f) {
			tCharacterInfo.fArmor -= pBullet->GetDamage();
			tCharacterInfo.fArmor = max(tCharacterInfo.fArmor, 0.f);
		}
		else {
			tCharacterInfo.fHp -= pBullet->GetDamage();
			tCharacterInfo.fHp = max(tCharacterInfo.fHp, 0.f);
		}
		if (tCharacterInfo.fHp <= 0.f) {
			Dead();
		}
		else {
			CGTA_Citizen* pCitizen = dynamic_cast<CGTA_Citizen*>(this);
			if (pCitizen) {
				pCitizen->Runaway();
				return;
			}
			CGTA_Cop* pCop = dynamic_cast<CGTA_Cop*>(this);
			if (pCop) {
				pCop->Trace();
				return;
			}
		}
	}
}

void CGTA_Character::OnCollisionStay(CObject* _pOther)
{
}

void CGTA_Character::OnCollisionExit(CObject* _pOther)
{
}

void CGTA_Character::DriveUpdate()
{
}

void CGTA_Character::State()
{
}

void CGTA_Character::Stun()
{
	SetCharacterState(E_CharacterState::stun);
	GetAI()->ChangeState(L"stun");
}

void CGTA_Character::Attack()
{
	E_WeaponType ecurWeaponType = GetCurWeaponType();
	TWeaponInfo& tWeaponInfo = m_vecWeapon[(UINT)m_eCurWeaponType].second;

	// 주먹이면
	if (E_WeaponType::FIST == ecurWeaponType) {
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

			CSound* pSound = CResourceManager::GetInstance()->GetSound(tWeaponInfo.strSound, tWeaponInfo.strSound);
			pSound->Play(false);
		}
	}
	else {
		// 총 타입에 따라 Shoot.
		// 총알 오브젝트 생성.
		CGTA_Bullet* pBullet = new CGTA_Bullet(E_GroupType::PROJECTILE);
		pBullet->Init();
		pBullet->SetDamage(tWeaponInfo.fDamage);
		float fDegree = GetRotateDegree();
		pBullet->SetRotateDegree(fDegree - 180); // 방향 설정
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
		// 타입별 사운드 재생
		//tWeaponInfo.strName
	}
	SetCharacterState(E_CharacterState::attack);
}

void CGTA_Character::Attack(Vector3 _TargetPos)
{
	Vector3 vDir = _TargetPos - GetPosition();
	vDir.Normalized();
	//RotateRP(vDir); // 캐릭터를 회전
	LookAt(_TargetPos, 100* DeltaTime);
	Attack();
}

void CGTA_Character::HitByFist()
{
	SetCharacterState(E_CharacterState::stun);
	SetAIState(E_AIState::stun);
}

void CGTA_Character::Drive()
{
	SetRender(false);
	GetCollider()->SetActive(false);
	m_pVehicle->SetDriver(this);
	SetCharacterState(E_CharacterState::drive);
	SetAIState(E_AIState::drive);
}

// TODO: 자동 타겟팅 구현하기
float CGTA_Character::AutoTargeting(const Vector3& _vUpDirVec, const Vector3& _vTargetDirVec)
{
	// 공격 시 범위 내에 있으면 자동으로 타겟하기
	float fDot = CMyMath::GetDot(_vUpDirVec, _vTargetDirVec);
	float fDegree = acosf(fDot) * CMyMath::Rad2Deg();

	return 0.f;
}

void CGTA_Character::Dead()
{
	SetCharacterState(E_CharacterState::dead);
	SetAIState(E_AIState::dead);
}

void CGTA_Character::GetInTheVehicle()
{
	SetCharacterState(E_CharacterState::getInTheVehicle);
	SetAIState(E_AIState::walkToCar);
}

void CGTA_Character::GetOutTheVehicle()
{
	SetRender(true);
	GetCollider()->SetActive(true);
	SetCharacterState(E_CharacterState::idle);
	SetAIState(E_AIState::wander);
	SetDrive(false);
	m_pVehicle->SetDriver(nullptr);
}

void CGTA_Character::ChangePrevWeapon()
{
	int iNextWeaponIdx = (int)m_eCurWeaponType + 1;
	while (true) {
		if (iNextWeaponIdx >= (int)E_WeaponType::END)
			iNextWeaponIdx = 0;

		if (true == m_vecWeapon[iNextWeaponIdx].first) {
			m_eCurWeaponType = (E_WeaponType)iNextWeaponIdx;
			break;
		}
		++iNextWeaponIdx;
	}

	const TWeaponInfo& tWeaponInfo = m_vecWeapon[(UINT)m_eCurWeaponType].second;
	m_fAttackMaxCoolTime = tWeaponInfo.fShootCoolTime;
}

void CGTA_Character::ChangeNextWeapon()
{
	int iPrevWeaponIdx = (int)m_eCurWeaponType - 1;
	while (true) {
		if (iPrevWeaponIdx < 0)
			iPrevWeaponIdx = (int)E_WeaponType::END - 1;

		if (true == m_vecWeapon[iPrevWeaponIdx].first) {
			m_eCurWeaponType = (E_WeaponType)iPrevWeaponIdx;
			break;
		}
		--iPrevWeaponIdx;
	}

	const TWeaponInfo& tWeaponInfo = m_vecWeapon[(UINT)m_eCurWeaponType].second;
	m_fAttackMaxCoolTime = tWeaponInfo.fShootCoolTime;
}

void CGTA_Character::GetItem(CGTA_Item* pItem)
{
	// 아이템을 얻으면
	if (E_ItemType::WEAPON == pItem->GetItemType()) {
		E_WeaponType eWeaponType = pItem->GetWeaponType();
		TWeaponInfo tWeaponInfo = GetWeaponInfo(eWeaponType);

		//총알 추가
		int iBullet = tWeaponInfo.iBulletCnt;
		tWeaponInfo = pItem->GetWeaponInfo();
		tWeaponInfo.iBulletCnt += iBullet;

		// 아이템을 이미 가지고 있다면 아이템의 총알을 더해준다.
		if (IsWeaponExists(eWeaponType))
			SetWeaponInfo(eWeaponType, tWeaponInfo);
		// 아이템이 없으면 그 아이템을 허용시키고 값들 대입 후, 현재 무기로 바꿔준다.
		else {
			SetWeaponState(true, eWeaponType); // 아이템 허용
			SetWeaponInfo(eWeaponType, tWeaponInfo); // 바뀐 정보 적용
			while (eWeaponType != GetCurWeaponType()) // 현재 무기로 바꿈
				ChangeNextWeapon();

			const TWeaponInfo& tCurWeaponInfo = m_vecWeapon[(UINT)m_eCurWeaponType].second;
			m_fAttackMaxCoolTime = tCurWeaponInfo.fShootCoolTime;
		}
	}
}

void CGTA_Character::Trace()
{
	SetCharacterState(E_CharacterState::run);
	SetAIState(E_AIState::trace);
}

void CGTA_Character::Wander()
{
	SetCharacterState(E_CharacterState::walk);
	SetAIState(E_AIState::wander);
}

void CGTA_Character::Runaway()
{
	SetCharacterState(E_CharacterState::run);
	SetAIState(E_AIState::runAway);
}

// 탈 수 있는 가까운 차량의 오브젝트를 리턴한다. 존재하지 않으면 nullptr 리턴
CGTA_Vehicle* CGTA_Character::FindNearbyVehicle()
{
	vector<CObject*>& vecVehicles = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::VEHICLE);

	CGTA_Vehicle* pVehicle = nullptr;
	float fMinDistance = m_fVehicleSearchDistance;

	for (UINT i = 0; i < vecVehicles.size(); ++i) {
		float distance = CMyMath::GetDistance(GetPosition(), vecVehicles[i]->GetPosition());
		if (distance < fMinDistance) {
			CGTA_Vehicle* pTempVehicle = (CGTA_Vehicle*)vecVehicles[i];
			if (false == pTempVehicle->DidExplode()) {
				distance = fMinDistance;
				pVehicle = pTempVehicle;
			}
		}
	}

	return pVehicle;
}

void CGTA_Character::CreateAI()
{
	if (nullptr == m_pAI) {
		m_pAI = new CGTA_AI();
		m_pAI->m_pCharacter = this;
	}
}

void CGTA_Character::CreatePathFinding()
{
	if (nullptr == m_pPathFinding) {
		m_pPathFinding = new CPathFinding();
		m_pPathFinding->Init();
	}
}

void CGTA_Character::SelectWeapon(E_WeaponType _eWeaponType)
{
	if (false == IsWeaponExists(_eWeaponType))
		return;
	while (_eWeaponType != GetCurWeaponType())
		ChangeNextWeapon();
}

void CGTA_Character::DropWeaponItem()
{
	// 현재 가지고 있는 아이템을 드랍한다.
	if (E_WeaponType::FIST != GetCurWeaponType()) {
		CGTA_Item* pItem = new CGTA_Item(E_GroupType::ITEM);
		pItem->Init();
		pItem->InitWeapon(GetCurWeaponType());
		TWeaponInfo tWeaponInfo;
		tWeaponInfo.InitWeapon(GetCurWeaponType());
		pItem->SetWeaponInfo(tWeaponInfo);
		pItem->SetPosition(GetPosition());
		CreateObject(pItem);
	}
}

void TWeaponInfo::Save(FILE* _pFile)
{
	SaveWString(strName, _pFile);
	fwrite(&fSplashRange, sizeof(float), 1, _pFile);
	fwrite(&fDamage, sizeof(float), 1, _pFile);
	fwrite(&iBulletCnt, sizeof(int), 1, _pFile);
	fwrite(&bSplashDamage, sizeof(bool), 1, _pFile);
	fwrite(&bIsInfinite, sizeof(bool), 1, _pFile);
	fwrite(&fShootCoolTime, sizeof(float), 1, _pFile);
	SaveWString(strSound, _pFile);
}

void TWeaponInfo::Load(FILE* _pFile)
{
	LoadWString(strName, _pFile);
	fread(&fSplashRange, sizeof(float), 1, _pFile);
	fread(&fDamage, sizeof(float), 1, _pFile);
	fread(&iBulletCnt, sizeof(int), 1, _pFile);
	fread(&bSplashDamage, sizeof(bool), 1, _pFile);
	fread(&bIsInfinite, sizeof(bool), 1, _pFile);
	fread(&fShootCoolTime, sizeof(float), 1, _pFile);
	LoadWString(strSound, _pFile);
}

TWeaponInfo::TWeaponInfo() : 
	strName(L""), 
	fSplashRange(20.f), 
	fDamage(1.f), 
	iBulletCnt(0), 
	bSplashDamage(false), 
	bIsInfinite(false), 
	fShootCoolTime(1.f),
	strSound(L"")
{
}

TWeaponInfo::TWeaponInfo(const TWeaponInfo& _other) :
	strName(_other.strName),
	fSplashRange(_other.fSplashRange),
	fDamage(_other.fDamage),
	iBulletCnt(_other.iBulletCnt),
	bSplashDamage(_other.bSplashDamage),
	bIsInfinite(_other.bIsInfinite),
	fShootCoolTime(_other.fShootCoolTime),
	strSound(_other.strSound)
{
}

void TWeaponInfo::InitWeapon(E_WeaponType _eWeaponType)
{
	switch (_eWeaponType) {
	case E_WeaponType::PISTOL:
		fDamage = 3.0f;
		strName = STR_NAME_Pistol;
		fShootCoolTime = 0.6f;
		iBulletCnt = 30;
		strSound = STR_FILE_PATH_GTA_Sound_Pistol;
		break;
	case E_WeaponType::ROCKET_LAUNCHER:
		fDamage = 100.f;
		strName = STR_NAME_RocketLauncher;
		fShootCoolTime = 1.0f;
		iBulletCnt = 10;
		strSound = STR_FILE_PATH_GTA_Sound_RocketLauncher;
		break;
	case E_WeaponType::SHOTGUN:
		fDamage = 20.f;
		strName = STR_NAME_Shotgun;
		fShootCoolTime = 1.2f;
		iBulletCnt = 20;
		strSound = STR_FILE_PATH_GTA_Sound_Shotgun;
		break;
	case E_WeaponType::SUBMACHINE_GUN:
		fDamage = 3.0f;
		strName = STR_NAME_SubmachineGun;
		fShootCoolTime = 0.13f;
		iBulletCnt = 300;
		strSound = STR_FILE_PATH_GTA_Sound_SubmachineGun;
		break;
	}
}

void TCharacterInfo::Save(FILE* _pFile)
{
	fwrite(&fHp, sizeof(float), 1, _pFile);
	fwrite(&fArmor, sizeof(float), 1, _pFile);
	fwrite(&fMoveSpeed, sizeof(float), 1, _pFile);
	fwrite(&fWalkSpeed, sizeof(float), 1, _pFile);
}

void TCharacterInfo::Load(FILE* _pFile)
{
	fread(&fHp, sizeof(float), 1, _pFile);
	fread(&fArmor, sizeof(float), 1, _pFile);
	fread(&fMoveSpeed, sizeof(float), 1, _pFile);
	fread(&fWalkSpeed, sizeof(float), 1, _pFile);
}
