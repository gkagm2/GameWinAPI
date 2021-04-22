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
	// TODO : �������� ����ұ�.. �������� ã�� �ؾߵǴµ�..
	// BFS�� �̿��Ͽ� ã���� �ұ�?
	// ���� �Ÿ��� ���Ƿ� �ؼ� �����̰� �ұ�??
	// 360���� �̿��Ͽ� �Ÿ��� ���Ѵ���  �ƳľƳ�.. ���� 4~5������ Tile�� ������������ BFS�� �̿��Ͽ� ���ϴ°� ���� �� ����. ���߿� �����ϱ�
	TTilePos destPos = GetRandomDestinationPos(m_iFindDepthMin, m_iFindDepthMax);

	// input�� �ؾ��ϴ°� �ƴ϶� �������� �������� �̿��Ͽ� 
	// �������� ��ã������
	if (false == GetCharacter()->GetPathFinding()->IsFoundDestination()) {
		// ���� ã�´�.
		m_bIsPathFind = GetCharacter()->GetPathFinding()->PathFind(startPos, destPos);
	}
	else {
		// �������� ã�Ұ� �� �������� ������ ���¸�
		if (true == GetCharacter()->GetPathFinding()->IsArrivedDestination()) {
			// �ٸ� ���� ã�´�.
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
	
	queue<TTilePos> que;
	que.push(tCurPos);
	int iDepth = 0;


	// TODO : �ߺ��� ���ֱ�
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
	
	// TODO : ������ �ߺ� ��� ����
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