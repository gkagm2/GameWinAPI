#include "stdafx.h"
#include "CGTA_WalkToVehicleState.h"
#include "CGTA_Vehicle.h"
#include "CTileMap.h"
#include "CPathFinding.h"
#include "CGTA_Character.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CTimeManager.h"

#include "CGTA_AI.h"
#include "CGTA_Player.h"

CGTA_WalkToVehicleState::CGTA_WalkToVehicleState() :
	m_pVehicle(nullptr),
	m_bIsFindPath(false)
{
}

CGTA_WalkToVehicleState::~CGTA_WalkToVehicleState()
{
}

void CGTA_WalkToVehicleState::Update()
{
	// 영역 안에 자동차가 있는지 확인한다.
	m_pVehicle = GetCharacter()->FindNearbyVehicle(); // FIXED : 플레이어인경우 이미 한번 함 여기서 또 확인하는것임
	if (m_pVehicle) { // 자동차가 있으면
		CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
		TTilePos tCurPos = pTileMap->VectorToTilePos(GetCharacter()->GetPosition());
		TTilePos tVehiclePos = pTileMap->VectorToTilePos(m_pVehicle->GetPosition());

		m_bIsFindPath = GetCharacter()->GetPathFinding()->FindPath(tCurPos, tVehiclePos); // 길찾기
	}
	else { // 자동차가 없으면
		// 다시 빠져나감.
		CGTA_Player* pPlayer = dynamic_cast<CGTA_Player*>(GetCharacter());
		if (pPlayer) { // 플레이어면
			pPlayer->SetActiveAI(false); // AI를 끈다.
			pPlayer->SetCharacterState(E_CharacterState::idle);
		}
		else { // 일반 시민이면
			GetCharacter()->GetAI()->ChangeState(L"wander");
		}
		return;
	}

	// 자동차가 있다면
	if (m_pVehicle) {
		GetCharacter()->SetMoveFlag(true);
		if (m_bIsFindPath) {
			GetAI()->Move(GetCharacter()->CharacterInfo().fMoveSpeed); // 찾았으면 걸어간다.

			if (false == GetCharacter()->GetPathFinding()->IsArrivedDestination())
				GetAI()->RotateBody();
			// TODO : 범위 체크, 끝까지 갔으면 차에 타기 // 이후에 없애기
		}
		else {
			float fDistance = CMyMath::GetDistance(GetCharacter()->GetPosition(), m_pVehicle->GetPosition());
			if (fDistance < 70.f) {
				Vector3 vCurPos = GetCharacter()->GetPosition();
				GetCharacter()->LookAt(m_pVehicle->GetPosition(), 400 * DeltaTime);
				// 그냥 움직이기
				Vector3 vDir = m_pVehicle->GetPosition() - GetCharacter()->GetPosition();
				vDir.Normalized();
				GetCharacter()->SetPosition(GetCharacter()->GetPosition() + vDir * GetCharacter()->CharacterInfo().fMoveSpeed * DeltaTime);
			}
		}
	}
}

void CGTA_WalkToVehicleState::Start()
{
}

void CGTA_WalkToVehicleState::End()
{
	m_pVehicle = nullptr;
}