#include "stdafx.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CTileMap.h"
#include "CTile.h"

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