#include "stdafx.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CTileMap.h"
#include "CTile.h"
#include "CObject.h"
#include "CColliderRect.h"
#include "CPathFinding.h"

CTileMap::CTileMap(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_iRow(0),
	m_iCol(0),
	m_iTileMapWidth(0),
	m_iTileMapHeight(0)

{
	CreatePathFinding();
}

CTileMap::~CTileMap()
{
}

void CTileMap::CreatePathFinding()
{
}

void CTileMap::Update()
{
}

void CTileMap::LateUpdate()
{
}

void CTileMap::Render(HDC _hDC)
{
}

void CTileMap::CreateTileGrid(UINT _iRow, UINT _iCol)
{
	m_iRow = _iRow;
	m_iCol = _iCol;
	m_iTileMapWidth = _iCol * CTile::GetTileSize();
	m_iTileMapHeight = _iRow * CTile::GetTileSize();
	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
	assert(pCurScene && L"null - current Scene");

	for (UINT y = 0; y < _iRow; ++y) {
		for (UINT x = 0; x < _iCol; ++x) {
			CTile* pTile = new CTile(E_GroupType::TILE);
			pTile->SetPosition(Vector2((float)x * CTile::GetTileSize(), (float)y * CTile::GetTileSize()));
			pCurScene->AddObject((CObject*)pTile);
		}
	}
}

void CTileMap::GetEndIdxOfRectArea(int** _grid, int _startX, int _startY, int& _endX, int& _endY) {

	vector<CObject*>& vecTileObj = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);

	CTile* pTile = dynamic_cast<CTile*>(vecTileObj[_startY * m_iCol + _startX]);

	// get min size of column and row
	int minX = (int)m_iCol - 1;
	int minY = (int)m_iRow - 1;
	int y = _startY;
	int x = _startX;
	for (; y < (int)m_iRow; ++y) {
		x = _startX;

		pTile = dynamic_cast<CTile*>(vecTileObj[y * m_iCol + x]);
		if (!(E_TileType::Wall == pTile->GetTileType() || E_TileType::Water == pTile->GetTileType()) || (int)E_VisitedState::visited == _grid[y][x]) {
			minY = y - 1;
			break;
		}

		for (; x <= minX; ++x) {
			pTile = dynamic_cast<CTile*>(vecTileObj[y * m_iCol + x]);
			if (!(E_TileType::Wall == pTile->GetTileType() || E_TileType::Water == pTile->GetTileType()) || (int)E_VisitedState::visited == _grid[y][x] ) {
				if (minX > x - 1) {
					minX = x - 1;
				}
			}
		}
	}

	// 잡힌 영역을 방문했다고 표시한다.
	for (int y = _startY; y <= minY; ++y) {
		for (int x = _startX; x <= minX; ++x) {
			_grid[y][x] = 1;
		}
	}
	x = _startX;
	y = _startY;

	_endX = minX;
	_endY = minY;
}

TTilePos CTileMap::VectorToTilePos(const Vector2& _vPos)
{
	return TTilePos { int(abs(_vPos.x / TILE_SIZE)), int(abs(_vPos.y / TILE_SIZE)) };
	
}

Vector2 CTileMap::TilePosToVector(const TTilePos& _tTilePos)
{
	return Vector2{ abs(_tTilePos.x * TILE_SIZE), abs(_tTilePos.y * TILE_SIZE) };
}

void CTileMap::OptimizationTileCollider()
{
	// 2차원 배열 동적할당.
	int **grid = new int* [m_iRow];
	for (int i = 0; i < (int)m_iRow; ++i)
		grid[i] = new int[(int)m_iCol];

	for (int i = 0; i < (int)m_iRow; ++i)
		memset(grid[i], 0, sizeof(int) * m_iCol);

	// 잡힌 영역을 저장할 벡터
	vector<CObject*>& vecTileObj = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);

	// grid type -> 0 : not visited, 1: visited
	vector<RECT> vecPos; // 콜라이더를 생성시킬 벡터
	// collider 영역 잡기.
	enum class E_VisitedState{ not_visited, visited};
	int startPosX = 0;
	int startPosY = 0;
	int endPosX = 0;
	int endPosY = 0;
	for (int y = 0; y < (int)m_iRow; ++y) {
		for (int x = 0; x < (int)m_iCol; ++x) {
			endPosX = startPosX = x;
			endPosY = startPosY = y;
			CTile* pTile = dynamic_cast<CTile*>(vecTileObj[startPosY * m_iCol + startPosX]);
			if ((E_TileType::Wall == pTile->GetTileType() || E_TileType::Water == pTile->GetTileType()) && (int)E_VisitedState::not_visited == grid[startPosY][startPosX]) {
				GetEndIdxOfRectArea(grid, startPosX, startPosY, endPosX, endPosY);
				vecPos.push_back(RECT{ startPosX,startPosY,endPosX,endPosY });
			}
			grid[startPosY][startPosX] = (int)E_VisitedState::visited;
		}
	}

	// 콜라이더 설정
	for (UINT i = 0; i < vecPos.size(); ++i) {
		RECT tRect = vecPos[i];

		CTile* pTile = dynamic_cast<CTile*>(vecTileObj[tRect.top* m_iCol + tRect.left]);
		if (pTile) {
			CColliderRect* pColRect = new CColliderRect(pTile);

			float scaleX = (float)TILE_SIZE * (tRect.right + 1 - tRect.left);
			float scaleY = (float)TILE_SIZE * (tRect.bottom + 1 - tRect.top);
			pColRect->SetScale(Vector3(scaleX, scaleY));
			pColRect->SetOffsetPosition(Vector3(scaleX * 0.5f, scaleY * 0.5f));
		}
	}

	for (int i = 0; i < (int)m_iRow; ++i) {
		delete[] grid[i];
	}
	delete[] grid;
}