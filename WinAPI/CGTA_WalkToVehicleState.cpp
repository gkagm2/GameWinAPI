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
	// ���� �ȿ� �ڵ����� �ִ��� Ȯ���Ѵ�.
	m_pVehicle = GetCharacter()->FindNearbyVehicle(); // FIXED : �÷��̾��ΰ�� �̹� �ѹ� �� ���⼭ �� Ȯ���ϴ°���
	if (m_pVehicle) { // �ڵ����� ������
		CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
		TTilePos tCurPos = pTileMap->VectorToTilePos(GetCharacter()->GetPosition());
		TTilePos tVehiclePos = pTileMap->VectorToTilePos(m_pVehicle->GetPosition());

		m_bIsFindPath = GetCharacter()->GetPathFinding()->FindPath(tCurPos, tVehiclePos); // ��ã��
	}
	else { // �ڵ����� ������
		// �ٽ� ��������.
		CGTA_Player* pPlayer = dynamic_cast<CGTA_Player*>(GetCharacter());
		if (pPlayer) { // �÷��̾��
			pPlayer->SetActiveAI(false); // AI�� ����.
			pPlayer->SetCharacterState(E_CharacterState::idle);
		}
		else { // �Ϲ� �ù��̸�
			GetCharacter()->GetAI()->ChangeState(L"wander");
		}
		return;
	}

	// �ڵ����� �ִٸ�
	if (m_pVehicle) {
		GetCharacter()->SetMoveFlag(true);
		if (m_bIsFindPath) {
			GetAI()->Move(GetCharacter()->CharacterInfo().fMoveSpeed); // ã������ �ɾ��.

			if (false == GetCharacter()->GetPathFinding()->IsArrivedDestination())
				GetAI()->RotateBody();
			// TODO : ���� üũ, ������ ������ ���� Ÿ�� // ���Ŀ� ���ֱ�
		}
		else {
			float fDistance = CMyMath::GetDistance(GetCharacter()->GetPosition(), m_pVehicle->GetPosition());
			if (fDistance < 70.f) {
				Vector3 vCurPos = GetCharacter()->GetPosition();
				GetCharacter()->LookAt(m_pVehicle->GetPosition(), 400 * DeltaTime);
				// �׳� �����̱�
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