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
	m_fMinSearchDistance(600.f),
	m_fMaxSearchDistance(800.f)
{
}

CGTA_WanderState::~CGTA_WanderState()
{
}
void CGTA_WanderState::Update()
{
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
	TTilePos tStartPos = pTileMap->VectorToTilePos(GetCharacter()->GetPosition());

	// input�� �ؾ��ϴ°� �ƴ϶� �������� �������� �̿��Ͽ� 
	// �������� ��ã������
	if (false == GetCharacter()->GetPathFinding()->IsFoundDestination()) {
		TTilePos destPos = GetRandomDestinationPos(m_fMinSearchDistance, m_fMaxSearchDistance);
		// ���� ã�´�.
		m_bIsPathFind = GetCharacter()->GetPathFinding()->FindPath(tStartPos, destPos);
	}
	else {
		// �������� ã�Ұ� �� �������� ������ ���¸�
		if (true == GetCharacter()->GetPathFinding()->IsArrivedDestination()) {
			TTilePos tDestPos = GetRandomDestinationPos(m_fMinSearchDistance, m_fMaxSearchDistance);
			// �ٸ� ���� ã�´�.
			m_bIsPathFind =GetCharacter()->GetPathFinding()->FindPath(tStartPos, tDestPos);
		}
	}
	// ��ο� ���� �̵��Ѵ�.
	if (m_bIsPathFind) {
		GetAI()->Move(GetCharacter()->CharacterInfo().fWalkSpeed);
		if (false == GetCharacter()->GetPathFinding()->IsArrivedDestination())
			GetAI()->RotateBody();
	}
}

void CGTA_WanderState::LateUpdate()
{
}

void CGTA_WanderState::Start()
{
}

void CGTA_WanderState::End()
{

}

TTilePos CGTA_WanderState::GetRandomDestinationPos(float _fMinSearchDistance, float _fMaxSearchDistance)
{
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
	TTilePos tCurPos = pTileMap->VectorToTilePos(GetCharacter()->GetPosition());
	vector<CObject*>& vecTiles = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);

	int iCol = (int)pTileMap->GetCol();
	int iRow = (int)pTileMap->GetRow();

	

	const set<E_TileType>& setTileType = GetCharacter()->GetPathFinding()->GetObstacleTiles();
	bool bIsObstacle = false;
	TTilePos tDestPos{tCurPos.x, tCurPos.y};
	int iTryCnt = 0;
	while (iTryCnt <= 10) {
		float fRandomDegree = float(rand() % 360);
		float fRandomMinDistance = float((rand() % (int)_fMaxSearchDistance) + _fMinSearchDistance); // 600���� 800 ����
		Vector3 vDir = { cosf(fRandomDegree), sinf(fRandomDegree), 0.f };

		Vector3 vDestPos = GetCharacter()->GetPosition() +  vDir * fRandomMinDistance;
		tDestPos = pTileMap->VectorToTilePos(vDestPos);
		
		if (GetCharacter()->GetPathFinding()->IsValid(tDestPos.x, tDestPos.y)) {
			// �� �� �ִ� Ÿ������ üũ�Ѵ�.
			for (auto iter = setTileType.begin(); iter != setTileType.end(); ++iter) {
				CTile* pTile = (CTile*)vecTiles[tDestPos.y * iCol + tDestPos.x];
				if (pTile->GetTileType() == *iter) {
					bIsObstacle = true;
					break;
				}
			}
			// �� �� ������ ����
			if (false == bIsObstacle)
				break;
		}
		
		++iTryCnt;
	}
	return tDestPos;
}

// BFS �̿��Ͽ� Delpth(Level) ������ ��ġ���� ����
TTilePos CGTA_WanderState::GetRandomDestinationPos(int _iDepthMin,int _iDepthMax)
{
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
	TTilePos tCurPos = pTileMap->VectorToTilePos(GetCharacter()->GetPosition());

	int iCol = (int)pTileMap->GetCol();
	int iRow = (int)pTileMap->GetRow();	

	int dirX[] = { 1, -1, 0, 0 };
	int dirY[] = { 0, 0, 1, -1 };

	vector<CObject*>& vecTiles = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);
	
	queue<std::pair<TTilePos, int>> que;
	int iDepth = 0;
	que.push(make_pair(tCurPos, iDepth));


	map<wstring, TTilePos> mapDestPos;

	while (!que.empty()) {
		tCurPos = que.front().first;
		iDepth = que.front().second;
		if (iDepth > _iDepthMax)
			break;
		que.pop();

		for (int i = 0; i < 4; ++i) {
			int x = tCurPos.x + dirX[i];
			int y = tCurPos.y + dirY[i];

			if (false == GetCharacter()->GetPathFinding()->IsValid(x, y))
				continue;

			const set<E_TileType>& setTileType = GetCharacter()->GetPathFinding()->GetObstacleTiles();
			bool bIsObstacle = false;

			// �� �� �ִ� Ÿ������ üũ�Ѵ�.
			for (auto iter = setTileType.begin(); iter != setTileType.end() ; ++iter) {
				CTile* pTile = (CTile*)vecTiles[y * iCol + x];
				if (pTile->GetTileType() == *iter) {
					bIsObstacle = true;
					break;
				}
			}

			if (bIsObstacle)
				continue;
			que.push(make_pair(TTilePos{ x,y }, iDepth + 1));
			if (_iDepthMin <= iDepth && _iDepthMax >= iDepth) {
				TTilePos tPos = { x,y };
				wstring key = std::to_wstring(x) + std::to_wstring(y);
				if(mapDestPos.find(key) != mapDestPos.end())
					continue;
				mapDestPos.insert(make_pair(key, tPos));
			}
		}
	}

	auto iter = mapDestPos.begin();

	int iSize = (int)mapDestPos.size();
	if (iSize == 0)
		return TTilePos{ tCurPos.x, tCurPos.y };

	int randIdx = rand() % iSize;
	for (int i = 0; i < randIdx; ++i)
		iter++;

	return iter->second;
}