#include "stdafx.h"
#include "CResourceManager.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CTile.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CColliderRect.h"

int CTile::g_iTileSize = TILE_SIZE;

CTile::CTile(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_iImageIdx(-1), // -1 Mean : none
	m_iMaxCol(0),
	m_iMaxRow(0),
	m_eTileType(E_TileType::None)
{
	/*CTexture* pTexture = CResourceManager::GetInstance()->GetTexture(STR_FILE_NAME_Tile, STR_FILE_PATH_Tile);
	SetTexture(pTexture);

	m_iMaxCol = pTexture->GetWidth() / g_iTileSize;
	m_iMaxRow = pTexture->GetHeight() / g_iTileSize;

	assert((0 != m_iMaxCol) && L"max Col is 0 (tile)");
	assert((0 != m_iMaxRow) && L"max Row is 0 (tile)");*/

	m_iMaxCol = 1;
	m_iMaxRow = 1;
}

CTile::~CTile()
{
}

void CTile::Update()
{
}

void CTile::Render(HDC _hDC)
{
	Vector3 vRenderPos = MainCamera->GetRenderPosition(GetPosition());

	if (nullptr == GetTexture() || -1 == m_iImageIdx)
		RenderDefaultTile(_hDC, vRenderPos);
	else {
		if (IsOutOfIdx()) {
			RenderDefaultTile(_hDC, vRenderPos);
			m_iImageIdx = -1;
			return;
		}
		// 이미지 드로잉.
		BitBlt(_hDC, (int)vRenderPos.x, (int)vRenderPos.y, g_iTileSize, g_iTileSize, GetTexture()->GetDC(), GetCol() * g_iTileSize, GetRow() * g_iTileSize, SRCCOPY);
	}

	if (GetCollider()) {
		if(GetCollider()->IsRender())
			GetCollider()->Render(_hDC);
	}
}

void CTile::RenderDefaultTile(HDC _hDC, const Vector3& _vRenderPos)
{
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, hBrush);
	Rectangle(_hDC, (int)_vRenderPos.x, (int)_vRenderPos.y, (int)_vRenderPos.x + GetTileSize(), (int)_vRenderPos.y + GetTileSize());
	SelectObject(_hDC, hOldBrush);
	DeleteObject(hBrush);
}

void CTile::SetTile(int _iImageIdx, E_TileType _eTileType, CTexture* _pTileTexture)
{
	m_iImageIdx = _iImageIdx;
	m_eTileType = _eTileType;
	SetTexture(_pTileTexture);
	m_iMaxCol = GetTexture()->GetWidth() / g_iTileSize;
	m_iMaxRow = GetTexture()->GetHeight() / g_iTileSize;
}

Vector3 CTile::GetMin()
{
	return GetPosition();
}

Vector3 CTile::GetMax()
{
	Vector3 vMaxPosition = GetPosition();
	CTexture* pTexture = GetTexture();
	if (nullptr != pTexture) {
		vMaxPosition.x += pTexture->GetWidth();
		vMaxPosition.y += pTexture->GetHeight();
	}
	else
		vMaxPosition += GetScale();
	return vMaxPosition;
}

void CTile::Save(FILE* _pFile)
{
	fwrite(&m_iImageIdx, sizeof(int), 1, _pFile);
	fwrite(&m_eTileType, sizeof(int), 1, _pFile);
}

void CTile::Load(FILE* _pFile)
{
	fread(&m_iImageIdx, sizeof(int), 1, _pFile);
	fread(&m_eTileType, sizeof(int), 1, _pFile);
	CTexture* pTexture = nullptr;
	switch (m_eTileType) {
	case E_TileType::Water:
		break;
	case E_TileType::Road: {
		pTexture = CResourceManager::GetInstance()->GetTexture(STR_FILE_PATH_UI_TileRoad, STR_FILE_PATH_UI_TileRoad);
	}
		break;
	case E_TileType::Sidewalk:
		break;
	case E_TileType::Wall: {
		pTexture = CResourceManager::GetInstance()->GetTexture(STR_FILE_PATH_UI_TileWall, STR_FILE_PATH_UI_TileWall);
		break;
	}
	}
	SetTexture(pTexture);
	m_iMaxCol = GetTexture()->GetWidth() / g_iTileSize;
	m_iMaxRow = GetTexture()->GetHeight() / g_iTileSize;
}