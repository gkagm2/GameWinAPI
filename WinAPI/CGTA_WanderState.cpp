#include "stdafx.h"
#include "CGTA_WanderState.h"
#include "CGTA_AI.h"
#include "CTileMap.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"
#include "CGTA_Character.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CCamera.h"
#include "CCore.h"

CGTA_WanderState::CGTA_WanderState() :
	m_pTarget(nullptr),
	m_vStartPos{},
	m_vDestPos{},
	m_bIsPathFind(false)
{
}

CGTA_WanderState::~CGTA_WanderState()
{
}
void CGTA_WanderState::Update()
{
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();

	TTilePos startPos = pTileMap->VectorToTilePos(GetAI()->GetCharacter()->GetPosition());
	// TODO : �������� ����ұ�.. �������� ã�� �ؾߵǴµ�..
	// BFS�� �̿��Ͽ� ã���� �ұ�?
	// ���� �Ÿ��� ���Ƿ� �ؼ� �����̰� �ұ�??
	// 360���� �̿��Ͽ� �Ÿ��� ���Ѵ���  �ƳľƳ�.. ���� 4~5������ Tile�� ������������ BFS�� �̿��Ͽ� ���ϴ°� ���� �� ����. ���߿� �����ϱ�
	TTilePos destPos = GetRandomDestinationPos();

	// input�� �ؾ��ϴ°� �ƴ϶� �������� �������� �̿��Ͽ� 
	// �������� ��ã������
	if (false == GetCharacter()->GetPathFinding()->IsFoundDestination()) {
		// ���� ã�´�.
		GetCharacter()->GetPathFinding()->PathFind(startPos, destPos);
	}
	else {
		// �������� ã�Ұ� �� �������� ������ ���¸�
		if (true == GetCharacter()->GetPathFinding()->IsArrivedDestination()) {
			// �ٸ� ���� ã�´�.
			GetCharacter()->GetPathFinding()->PathFind(startPos, destPos);
		}
	}

	if (InputKeyPress(E_Key::Ctrl)) {
		m_bIsPathFind = GetAI()->GetCharacter()->GetPathFinding()->PathFind(startPos, destPos);
	}
	if (m_bIsPathFind) {
		HDC _hDC = CCore::GetInstance()->GetDC();
		list<TTilePos>& path = GetAI()->GetCharacter()->GetPathFinding()->GetPath();

		list<TTilePos>::iterator iter = path.begin();
		if (GetAI()->GetCharacter()->GetPathFinding()->IsArrivedDestination())
			return;
		TTilePos prev = *iter;

		++iter;
		for (; iter != path.end(); ++iter) {
			Vector2 vCurPos = MainCamera->GetRenderPosition(pTileMap->TilePosToVector(prev));
			vCurPos += (TILE_SIZE * 0.5f);
			Rectangle(_hDC, vCurPos.x - 5, vCurPos.y - 5, vCurPos.x + 5, vCurPos.y + 5);
			MoveToEx(_hDC, vCurPos.x, vCurPos.y, nullptr);
			Vector2 destPos = MainCamera->GetRenderPosition(pTileMap->TilePosToVector(*iter));

			destPos += (TILE_SIZE * 0.5f);
			LineTo(_hDC, destPos.x, destPos.y);
			Ellipse(_hDC, destPos.x - 5, destPos.y - 5, destPos.x + 5, destPos.y + 5);
			prev = *iter;
		}
	}

	if (m_bIsPathFind) {
		list<TTilePos>& path = GetAI()->GetCharacter()->GetPathFinding()->GetPath();
		// �������� �����ߴ��� Ȯ���Ѵ�.
		// �������� ����������
		if (GetAI()->GetCharacter()->GetPathFinding()->IsArrivedDestination()) {
			return;
		}
		else { // �������� �������� �ʾ�����

			// ���ؾߵ���?
			// ������ ���� �� ��ġ�� �����´�.
			Vector2 vCurPos = GetAI()->GetCharacter()->GetPosition();

			// ��ġ�� ���ٸ�
			Vector2 vNextPos = pTileMap->TilePosToVector(path.front());
			vNextPos += TILE_SIZE * 0.5f;

			if (abs(vNextPos.x - vCurPos.x) < 15 && abs(vNextPos.y - vCurPos.y) < 15) {
				path.pop_front(); // �����ش�.
			}
			else { // ��ġ�� ���� �ʴٸ�
				// �� ��ġ�� �̵��Ѵ�.
				// 1.  ���� ���ϱ�
				Vector2 vDir = Vector2(vNextPos.x - vCurPos.x, vNextPos.y - vCurPos.y);
				vDir.Normalized();

				// �̵�
				GetAI()->GetCharacter()->SetPosition(GetAI()->GetCharacter()->GetPosition() + vDir * 300.f * DeltaTime);
			}
		}
	}
}

void CGTA_WanderState::LateUpdate()
{
}

void CGTA_WanderState::Start()
{
	GetCharacter()->Wander();
}

void CGTA_WanderState::End()
{

}

TTilePos CGTA_WanderState::GetRandomDestinationPos()
{
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();

	// TODO �����ϰ� �Ͼ�� �ʴ� ���� ������ �����ߵɰŰ�����..
	float fDistance = (float)(rand() % 10 + 50);
	float fDegree = float(rand() % 360);
	float fX = cosf(fDegree);
	float fY = sinf(fDegree);
	return pTileMap->VectorToTilePos({ fX,fY });
}