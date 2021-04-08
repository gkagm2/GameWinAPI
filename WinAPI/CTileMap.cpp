#include "stdafx.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CTileMap.h"
#include "CTile.h"
#include "CObject.h"

CTileMap::CTileMap(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_iRow(0),
	m_iCol(0),
	m_iTileMapWidth(0),
	m_iTileMapHeight(0)
{
}

CTileMap::~CTileMap()
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

// TODO : 타일 콜라이더 최적화하기
void CTileMap::GetEndIdxOfRectArea(int** _grid, int _startX, int _startY, int& _endX, int& _endY) {
	if (0 == _grid[_startY][_startX]) {
		_grid[_startY][_startX] = 1;
	}

	// 오른쪽 끝까지 간다.
	int startX = _startX;
	int startY = _startY;

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
	vector<CObject*>& tiles = pCurScene->GetObjects(E_GroupType::TILE);

	while (true) {
		for (int y = startY; y < m_iRow; ++y) {
			for (int x = startX; x < m_iCol; ++x) {
				CTile* pTile = (CTile*)tiles[m_iCol * y + x];
				if (E_TileType::Wall == pTile->GetTileType()) {
					
				}
			}
		}
	}
}

void CTileMap::OptimizationTileCollider()
{
	// 2차원 배열 동적할당.
	int **grid = new int* [m_iRow];
	for (int i = 0; i < m_iRow; ++i)
		grid[i] = new int[m_iCol];


	for (int i = 0; i < m_iRow; ++i)
		memset(grid[i], 0, sizeof(int) * m_iCol);

	// grid type -> 0 : non visited, 1: visited
	
	// collider 영역 잡기.
	int startPosX = 0;
	int startPosY = 0;
	int endPosX = 0;
	int endPosY = 0;
	for (int y = 0; y < m_iRow; ++y) {
		for (int x = 0; x < m_iCol; ++x) {
			endPosX = startPosX = x;
			endPosY = startPosY = y;
			if ((int)E_TileType::Wall == grid[startPosY][startPosX]) {
				GetEndIdxOfRectArea(grid, startPosX, startPosX, endPosX, endPosY);

				// start Pos 와 end Pos idx를 이용하여
				// 콜라이더 생성
				// 맨 왼쪽 위의 타일에 콜라이더를 생성한다.
			}
			grid[startPosY][startPosX] = 1;
		}
	}

	for (int i = 0; i < m_iRow; ++i) {
		delete[] grid[i];
	}
	delete[] grid;
}