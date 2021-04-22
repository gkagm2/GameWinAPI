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
#include "CTile.h"

CGTA_WanderState::CGTA_WanderState() :
	m_pTarget(nullptr),
	m_vStartPos{},
	m_vDestPos{},
	m_bIsPathFind(false),
	m_iFindDepthMin(30),
	m_iFindDepthMax(40)
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
	TTilePos destPos = GetRandomDestinationPos(m_iFindDepthMin, m_iFindDepthMax);

	// input을 해야하는게 아니라 이제부터 랜덤값을 이용하여 
	// 목적지를 못찾았으면
	if (false == GetCharacter()->GetPathFinding()->IsFoundDestination()) {
		// 길을 찾는다.
		m_bIsPathFind = GetCharacter()->GetPathFinding()->PathFind(startPos, destPos);
	}
	else {
		// 목적지를 찾았고 그 목적지에 도착한 상태면
		if (true == GetCharacter()->GetPathFinding()->IsArrivedDestination()) {
			// 다른 길을 찾는다.
			m_bIsPathFind =GetCharacter()->GetPathFinding()->PathFind(startPos, destPos);
		}
		else {

		}
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
			Rectangle(_hDC, (int)vCurPos.x - 5, (int)vCurPos.y - 5, (int)vCurPos.x + 5, (int)vCurPos.y + 5);
			MoveToEx(_hDC, (int)vCurPos.x, (int)vCurPos.y, nullptr);
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
				GetAI()->GetCharacter()->SetPosition(GetAI()->GetCharacter()->GetPosition() + vDir * 150.f * DeltaTime);
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

// BFS 이용하여 Delpth(Level) 사이의 위치값을 리턴
TTilePos CGTA_WanderState::GetRandomDestinationPos(int _iDepthMin,int _iDepthMax)
{
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();

	TTilePos tCurPos = pTileMap->VectorToTilePos(GetCharacter()->GetPosition());

	int iCol = (int)pTileMap->GetCol();
	int iRow = (int)pTileMap->GetRow();	

	int dirX[] = { 1, -1, 0, 0 };
	int dirY[] = { 0, 0, 1, -1 };

	vector<CObject*>& vecTiles = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);
	
	queue<TTilePos> que;
	que.push(tCurPos);
	int iDepth = 0;


	// TODO : 중복값 없애기
	map<int, TTilePos> com;

	while (!que.empty()) {
		if (iDepth > _iDepthMax)
			break;

		for (int i = 0; i < 4; ++i) {
			int x = tCurPos.x + dirX[i];
			int y = tCurPos.y + dirY[i];

			if (false == GetCharacter()->GetPathFinding()->IsValid(x, y))
				continue;

			const vector<E_TileType>& vecTileType = GetCharacter()->GetPathFinding()->GetObstacleTiles();
			bool bIsObstacle = false;

			for (int type = 0; type < vecTileType.size(); ++type) {
				CTile* pTile = (CTile*)vecTiles[y * iCol + x];
				if (pTile->GetTileType() == vecTileType[type]) {
					bIsObstacle = true;
					break;
				}
			}

			if (bIsObstacle)
				continue;
			que.push(TTilePos{ x,y });
			if (_iDepthMin <= iDepth && _iDepthMax >= iDepth) {
				TTilePos tPos = { x,y };
				auto iter = com.begin();
				for (; iter != com.end(); ++iter) {
					if (tPos == (*iter).second) {
						continue;
					}
				}
				com.insert(make_pair(tPos.x + 123 + tPos.x + 321 + tPos.y +234 + tPos.y + 345, tPos));
			}
		}
		++iDepth;
	}
	
	// TODO : 벡터의 중복 요소 없앰
	auto iter = com.begin();


	int iSize = (int)com.size();
	if (iSize == 0)
		return TTilePos{ tCurPos.x, tCurPos.y };

	int randIdx = rand() % iSize;

	for (int i = 0; i < randIdx; ++i) {
		iter++;
	}

	return iter->second;
}