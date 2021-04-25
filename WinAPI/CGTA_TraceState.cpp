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
	m_bIsPathFind(false)

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
				GetAI()->Move(GetCharacter()->CharacterInfo().fMoveSpeed * 0.7f);
				if (false == GetCharacter()->GetPathFinding()->IsArrivedDestination())
					GetAI()->RotateBody();
			}
		}
		else {
			if (fDistance <= 20.f) {
				// 멈춰서 때린다.
				GetCharacter()->Attack(m_pTarget->GetPosition());
			}
			else {
				// 그냥 움직이기
				Vector3 vDir = m_pTarget->GetPosition() - GetCharacter()->GetPosition();
				vDir.Normalized();
				GetCharacter()->SetPosition(GetCharacter()->GetPosition() + vDir * GetCharacter()->CharacterInfo().fMoveSpeed* 0.7f * DeltaTime);
			}
		}
	}
	else { // 총기류이면
		// 200미터 거리 밖이면 추적한다.
		if (fDistance >= m_fShootDistance) {
			if (m_bIsPathFind) {
				GetAI()->Move(GetCharacter()->CharacterInfo().fMoveSpeed * 0.7f);
				if (false == GetCharacter()->GetPathFinding()->IsArrivedDestination())
					GetAI()->RotateBody();
			}
				
		}
		// 200미터 거리 안이면 총을 쏜다.
		else
			GetCharacter()->Attack(m_pTarget->GetPosition());
	}
}

void CGTA_TraceState::LateUpdate()
{
}

void CGTA_TraceState::Start()
{
	if (nullptr == m_pTarget)
		m_pTarget = CSceneManager::GetInstance()->GetCurScene()->FindObject(L"Player");
}

void CGTA_TraceState::End()
{
}