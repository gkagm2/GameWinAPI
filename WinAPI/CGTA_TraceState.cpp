#include "stdafx.h"
#include "CGTA_TraceState.h"
#include "CResourceManager.h"
#include "CResource.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"
#include "CGTA_Player.h"
#include "CGTA_Character.h"
#include "CTimeManager.h"
#include "CPathFinding.h"
#include "CTileMap.h"
#include "CGtA_AI.h"
#include "CSound.h"

CGTA_TraceState::CGTA_TraceState() :
	m_pTarget(nullptr),
	m_fFistDistance(20.f),
	m_fDistance(300.f),
	m_fShootDistance(300.f),
	m_bIsPathFind(false),
	m_fAttackCoolTime(0.f),
	m_fAttackMaxCoolTime(1.5f),
	m_fISaidFreezeCoolTime(0.f),
	m_fISaidFreezeMaxCoolTime(5.f)
{
}

CGTA_TraceState::~CGTA_TraceState()
{
}

void CGTA_TraceState::Update()
{
	if (nullptr == m_pTarget)
		return;
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();

	TTilePos tCurPos = pTileMap->VectorToTilePos(GetCharacter()->GetPosition());
	TTilePos tDestPos = pTileMap->VectorToTilePos(m_pTarget->GetPosition());
	float fDistance = (m_pTarget->GetPosition() - GetCharacter()->GetPosition()).GetDistance();

	E_WeaponType eWeaponType = GetCharacter()->GetCurWeaponType();
	m_bIsPathFind = GetCharacter()->GetPathFinding()->FindPath(tCurPos, tDestPos);
	if (E_WeaponType::FIST == eWeaponType) { // 주먹이면
		if (fDistance >= 40.f) { 
			// 경로따라 움직이기
			if (m_bIsPathFind) {
				GetAI()->Move(GetCharacter()->CharacterInfo().fMoveSpeed);
				if (false == GetCharacter()->GetPathFinding()->IsArrivedDestination())
					GetAI()->RotateBody();
			}
			else {
				// 그냥 움직이기
				Vector3 vDir = m_pTarget->GetPosition() - GetCharacter()->GetPosition();
				vDir.Normalized();
				GetCharacter()->SetPosition(GetCharacter()->GetPosition() + vDir * GetCharacter()->CharacterInfo().fMoveSpeed * DeltaTime);
				GetCharacter()->SetCharacterState(E_CharacterState::run);
				if (false == GetCharacter()->GetPathFinding()->IsArrivedDestination())
					GetAI()->RotateBody();
			}
		}
		else {
			if (fDistance <= 20.f) {
				CGTA_Character* pCharacter =dynamic_cast<CGTA_Character*>(m_pTarget);
				if (pCharacter) {
					if (E_CharacterState::dead == pCharacter->GetCharacterState())
						GetCharacter()->Wander();
				}
				//CGTA_Vehicle* pVehicle = dynamic_cast<CGTA_Vehicle>*(m_pTarget);
				// 사정거리 내로 왔으면
				if (E_WeaponType::FIST == GetCharacter()->GetCurWeaponType()) {
					GetCharacter()->Attack(m_pTarget->GetPosition());
				}
				else {
					m_fAttackCoolTime += DeltaTime;
					GetCharacter()->LookAt(m_pTarget->GetPosition(), 400 * DeltaTime);
					if (m_fAttackCoolTime >= m_fAttackMaxCoolTime) {
						GetCharacter()->Attack(m_pTarget->GetPosition());
						m_fAttackCoolTime = 0.f;
					}
					GetCharacter()->SetCharacterState(E_CharacterState::idle);
				}
			}
			else {
				// 그냥 움직이기
				Vector3 vDir = m_pTarget->GetPosition() - GetCharacter()->GetPosition();
				vDir.Normalized();
				GetCharacter()->SetPosition(GetCharacter()->GetPosition() + vDir * GetCharacter()->CharacterInfo().fMoveSpeed * DeltaTime);
				GetCharacter()->SetCharacterState(E_CharacterState::run);
				if (false == GetCharacter()->GetPathFinding()->IsArrivedDestination())
					GetAI()->RotateBody();
			}
		}
	}
	else { // 총기류이면
		// 200미터 거리 밖이면 추적한다.
		if (fDistance >= m_fShootDistance) {
			if (m_bIsPathFind) {
				GetAI()->Move(GetCharacter()->CharacterInfo().fMoveSpeed);
				if (false == GetCharacter()->GetPathFinding()->IsArrivedDestination())
					GetAI()->RotateBody();
			}
			GetCharacter()->SetCharacterState(E_CharacterState::run);
		}
		// 200미터 거리 안이면 총을 쏜다.
		else {
			CGTA_Character* pCharacter = dynamic_cast<CGTA_Character*>(m_pTarget);
			if (pCharacter) {
				if (E_CharacterState::dead == pCharacter->GetCharacterState())
					GetCharacter()->Wander();
			}
			m_fAttackCoolTime += DeltaTime;
			GetCharacter()->LookAt(m_pTarget->GetPosition(), 400 * DeltaTime);
			if (m_fAttackCoolTime >= m_fAttackMaxCoolTime) {
				GetCharacter()->Attack(m_pTarget->GetPosition());
				m_fAttackCoolTime = 0.f;
			}
			GetCharacter()->SetCharacterState(E_CharacterState::idle);
		}
	}
	m_fISaidFreezeCoolTime += DeltaTime;
	if (m_fISaidFreezeCoolTime > m_fISaidFreezeMaxCoolTime) {
		wstring strISaidFreezeSoundPath = Sound_ISaidFreeze + std::to_wstring((rand() % Sound_ISaidFreeze_Len) + 1);
		CSound* pSound = CResourceManager::GetInstance()->GetSound(strISaidFreezeSoundPath, strISaidFreezeSoundPath);
		pSound->Play();
		m_fISaidFreezeCoolTime = 0.f;
	}
}

void CGTA_TraceState::LateUpdate()
{
}

void CGTA_TraceState::Start()
{
	if (nullptr == m_pTarget)
		m_pTarget = CSceneManager::GetInstance()->GetCurScene()->FindObject(STR_OBJECT_NAME_Player);
	m_fAttackMaxCoolTime = GetCharacter()->GetAttackMaxCoolTime();

	m_fISaidFreezeCoolTime = 0.f;
	m_fISaidFreezeMaxCoolTime = (float)((rand() % 30) + 5);
}

void CGTA_TraceState::End()
{
}