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

CGTA_TraceState::CGTA_TraceState() :
	m_pTarget(nullptr),
	m_fFistDistance(20.f),
	m_fDistance(300.f),
	m_fShootDistance(300.f),
	m_bIsPathFind(false),
	m_fAttackCoolTime(0.f),
	m_fAttackMaxCoolTime(1.f)

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
	if (E_WeaponType::FIST == eWeaponType) { // �ָ��̸�
		if (fDistance >= 40.f) { 
			// ��ε��� �����̱�
			if (m_bIsPathFind) {
				GetAI()->Move(GetCharacter()->CharacterInfo().fMoveSpeed * 0.7f);
				if (false == GetCharacter()->GetPathFinding()->IsArrivedDestination())
					GetAI()->RotateBody();
			}
			else {
				// �׳� �����̱�
				Vector3 vDir = m_pTarget->GetPosition() - GetCharacter()->GetPosition();
				vDir.Normalized();
				GetCharacter()->SetPosition(GetCharacter()->GetPosition() + vDir * GetCharacter()->CharacterInfo().fMoveSpeed * 0.7f * DeltaTime);
				GetCharacter()->SetCharacterState(E_CharacterState::run);
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
				// �����Ÿ� ���� ������
				m_fAttackCoolTime += DeltaTime;
				GetCharacter()->LookAt(m_pTarget->GetPosition(), 400 * DeltaTime);
				if (m_fAttackCoolTime >= m_fAttackMaxCoolTime) {
					GetCharacter()->Attack(m_pTarget->GetPosition());
					m_fAttackCoolTime = 0.f;
				}
				GetCharacter()->SetCharacterState(E_CharacterState::idle);
			}
			else {
				// �׳� �����̱�
				Vector3 vDir = m_pTarget->GetPosition() - GetCharacter()->GetPosition();
				vDir.Normalized();
				GetCharacter()->SetPosition(GetCharacter()->GetPosition() + vDir * GetCharacter()->CharacterInfo().fMoveSpeed* 0.7f * DeltaTime);
				GetCharacter()->SetCharacterState(E_CharacterState::run);
			}
		}
	}
	else { // �ѱ���̸�
		// 200���� �Ÿ� ���̸� �����Ѵ�.
		if (fDistance >= m_fShootDistance) {
			if (m_bIsPathFind) {
				GetAI()->Move(GetCharacter()->CharacterInfo().fMoveSpeed * 0.7f);
				if (false == GetCharacter()->GetPathFinding()->IsArrivedDestination())
					GetAI()->RotateBody();
			}
			GetCharacter()->SetCharacterState(E_CharacterState::run);
		}
		// 200���� �Ÿ� ���̸� ���� ���.
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
}

void CGTA_TraceState::LateUpdate()
{
}

void CGTA_TraceState::Start()
{
	if (nullptr == m_pTarget)
		m_pTarget = CSceneManager::GetInstance()->GetCurScene()->FindObject(L"Player");
	m_fAttackMaxCoolTime = GetCharacter()->GetAttackMaxCoolTime();
}

void CGTA_TraceState::End()
{
}