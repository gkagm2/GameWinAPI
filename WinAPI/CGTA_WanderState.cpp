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
	// TODO : 목적지를 어떻게할까.. 랜덤으로 찾게 해야되는데..
	// BFS를 이용하여 찾도록 할까?
	// 랜덤 거리를 임의로 해서 움직이게 할까??
	// 360도를 이용하여 거리를 구한다음  아냐아냐.. 대충 4~5정도의 Tile이 떨어진곳에서 BFS를 이용하여 구하는게 좋을 것 같다. 나중에 수정하기
	TTilePos destPos = GetRandomDestinationPos();

	// input을 해야하는게 아니라 이제부터 랜덤값을 이용하여 
	// 목적지를 못찾았으면
	if (false == GetCharacter()->GetPathFinding()->IsFoundDestination()) {
		// 길을 찾는다.
		GetCharacter()->GetPathFinding()->PathFind(startPos, destPos);
	}
	else {
		// 목적지를 찾았고 그 목적지에 도착한 상태면
		if (true == GetCharacter()->GetPathFinding()->IsArrivedDestination()) {
			// 다른 길을 찾는다.
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
		// 목적지에 도착했는지 확인한다.
		// 목적지에 도착했으면
		if (GetAI()->GetCharacter()->GetPathFinding()->IsArrivedDestination()) {
			return;
		}
		else { // 목적지에 도착하지 않았으면

			// 뭘해야되지?
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

	// TODO 과부하가 일어나지 않는 범위 내에서 만들어야될거같은데..
	float fDistance = (float)(rand() % 10 + 50);
	float fDegree = float(rand() % 360);
	float fX = cosf(fDegree);
	float fY = sinf(fDegree);
	return pTileMap->VectorToTilePos({ fX,fY });
}