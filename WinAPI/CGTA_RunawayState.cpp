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

CGTA_RunawayState::CGTA_RunawayState() :
	m_pTarget(nullptr),
	m_fMaxRunDistance(800.f),
	m_bIsPathFind(false),
	m_fPathFindCoolTime(0.7f),
	m_fPathFindMaxCoolTime(0.3f)
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

	// 내부에 있으면
	if (fDistance <= m_fMaxRunDistance) {
		CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
		// 계속해서 도망친다.
		// 오브젝트의 반대 방향으로 도망친다.
		Vector3 vDir = vPosition - m_pTarget->GetPosition();
		vDir.Normalized();
		
		TTilePos tCurPos = pTileMap->VectorToTilePos(GetCharacter()->GetPosition());
		TTilePos tRunDestPos = pTileMap->VectorToTilePos(GetCharacter()->GetPosition() + vDir * 100.f);

		m_fPathFindCoolTime += DeltaTime;
		if (m_fPathFindCoolTime >= m_fPathFindMaxCoolTime) {
			float angle = 0;
			while (angle < 180) {
				m_bIsPathFind = GetCharacter()->GetPathFinding()->PathFind(tCurPos, tRunDestPos);
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
			
		if (m_bIsPathFind) {
			// 경로에 따라 이동한다.
			list<TTilePos>& path = GetAI()->GetCharacter()->GetPathFinding()->GetPath();
			// 목적지에 도착했는지 확인한다.
			// 목적지에 도착했으면
			if (GetAI()->GetCharacter()->GetPathFinding()->IsArrivedDestination()) {
				return;
			}
			else { // 목적지에 도착하지 않았으면
				// 다음에 가야 할 위치를 가져온다.
				Vector2 vCurPos = GetAI()->GetCharacter()->GetPosition();

				// 위치가 같다면
				Vector2 vNextPos = pTileMap->TilePosToVector(path.front());
				vNextPos += TILE_SIZE * 0.5f;

				if (abs(vNextPos.x - vCurPos.x) < 15 && abs(vNextPos.y - vCurPos.y) < 15) {
					path.pop_front(); // 팝해준다.
				}
				else { // 위치가 같지 않다면
					// 그 위치로 이동한다.
					// 1.  방향 구하기
					Vector2 vDir = Vector2(vNextPos.x - vCurPos.x, vNextPos.y - vCurPos.y);
					vDir.Normalized();



					// 이동
					GetAI()->GetCharacter()->SetPosition(GetAI()->GetCharacter()->GetPosition() + vDir * GetCharacter()->CharacterInfo().fMoveSpeed * 0.7f * DeltaTime);
				}
			}
		}
	}
	else {
		GetCharacter()->Wander();
	}
}

void CGTA_RunawayState::LateUpdate()
{
}

void CGTA_RunawayState::Start()
{
	if (m_pTarget == nullptr) {
		m_pTarget = CSceneManager::GetInstance()->GetCurScene()->FindObject(L"Player");
	}
}

void CGTA_RunawayState::End()
{
}