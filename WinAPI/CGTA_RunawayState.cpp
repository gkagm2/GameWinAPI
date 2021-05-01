#include "stdafx.h"
#include "CGTA_RunawayState.h"
#include "CGTA_AIState.h"
#include "CObject.h"
#include "CGTA_Character.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CTileMap.h"
#include "CPathFinding.h"
#include "CTile.h"
#include "CCamera.h"
#include "CCore.h"
#include "CGTA_AI.h"
#include "CResourceManager.h"
#include "CSound.h"

CGTA_RunawayState::CGTA_RunawayState() :
	m_pTarget(nullptr),
	m_fMaxRunDistance(800.f),
	m_bIsPathFind(false),
	m_fPathFindCoolTime(0.7f),
	m_fPathFindMaxCoolTime(0.3f),
	m_fHelpSoundCoolTime(0.f),
	m_fHelpSoundMaxCoolTime(10.f)
{
}

CGTA_RunawayState::~CGTA_RunawayState()
{
}

void CGTA_RunawayState::Update()
{
	if (nullptr == m_pTarget)
		return;

	// m_pTarget으로부터 도망친다.
	Vector3 vPosition = GetCharacter()->GetPosition();
	
	float fDistance = (vPosition - m_pTarget->GetPosition()).GetDistance();

	// 내부에 없으면
	if (fDistance > m_fMaxRunDistance) {
		GetCharacter()->Wander();
	}
	else { // 내부에 있으면
		CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
		// 계속해서 도망친다.
		// 오브젝트의 반대 방향으로 도망친다.
		Vector3 vDir = vPosition - m_pTarget->GetPosition();
		vDir.Normalized();
		
		TTilePos tCurPos = pTileMap->VectorToTilePos(GetCharacter()->GetPosition());
		TTilePos tRunDestPos = pTileMap->VectorToTilePos(GetCharacter()->GetPosition() + vDir * 100.f);

		m_fPathFindCoolTime += DeltaTime;
		if (m_fPathFindCoolTime >= m_fPathFindMaxCoolTime) {
			// 각도에 따른 도망 칠 경로 계산
			float angle = 0;
			while (angle < 180) {
				m_bIsPathFind = GetCharacter()->GetPathFinding()->FindPath(tCurPos, tRunDestPos);
				if (m_bIsPathFind)
					break;

				Vector3 vRightDir = Rotate(vDir, angle);
				Vector3 vLeftDir = Rotate(vDir, -angle);
				Vector3 vLeftDestPos = GetCharacter()->GetPosition() + vLeftDir * 200.f;
				Vector3 vRightDestPos = GetCharacter()->GetPosition() + vRightDir * 200.f;
				TTilePos tLeftPos = pTileMap->VectorToTilePos(vLeftDestPos);
				TTilePos tRightPos = pTileMap->VectorToTilePos(vRightDestPos);

				bool bLeftSideIsObstacle = GetCharacter()->GetPathFinding()->IsObstacle(tLeftPos.x, tLeftPos.y);
				bool bRightSideIsObstacle = GetCharacter()->GetPathFinding()->IsObstacle(tRightPos.x, tRightPos.y);

				if (false == bLeftSideIsObstacle && false == bRightSideIsObstacle) {
					float fLeftDestDis = (GetCharacter()->GetPosition() - vLeftDestPos).GetDistance();
					float fRightDestDis = (GetCharacter()->GetPosition() - vRightDestPos).GetDistance();

					if (fLeftDestDis > fRightDestDis)
						vDir = vLeftDir;
					else
						vDir = vRightDir;
				}
				else if (false == bLeftSideIsObstacle)
					vDir = vLeftDir;
				else if(false == bRightSideIsObstacle)
					vDir = vRightDir;

				tRunDestPos = pTileMap->VectorToTilePos(GetCharacter()->GetPosition() + vDir * 200.f);
				angle += 5;
			}

			m_fPathFindCoolTime = 0.f;
		}
			
		// 경로에 따라 이동한다.
		if (m_bIsPathFind) {
			GetAI()->Move(GetCharacter()->CharacterInfo().fMoveSpeed);
			if (false == GetCharacter()->GetPathFinding()->IsArrivedDestination())
				GetAI()->RotateBody();
		}
	}

	m_fHelpSoundCoolTime += DeltaTime;
	if (m_fHelpSoundCoolTime > m_fHelpSoundMaxCoolTime) {
		wstring strHelpSoundPath = Sound_Help + std::to_wstring((rand() % Sound_Help_Len) + 1);
		CSound* pSound = CResourceManager::GetInstance()->GetSound(strHelpSoundPath, strHelpSoundPath);
		pSound->Play();
		m_fHelpSoundMaxCoolTime = rand() % 15 + 7;
		m_fHelpSoundCoolTime = 0;
	}
}

void CGTA_RunawayState::LateUpdate()
{
}

void CGTA_RunawayState::Start()
{
	if (m_pTarget == nullptr) {
		m_pTarget = CSceneManager::GetInstance()->GetCurScene()->FindObject(STR_OBJECT_NAME_Player);
	}

	m_fHelpSoundCoolTime = 0;
	m_fHelpSoundMaxCoolTime = rand() % 15 + 5;
}

void CGTA_RunawayState::End()
{
}