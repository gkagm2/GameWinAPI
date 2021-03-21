#include "stdafx.h"
#include "CResourceManager.h"

#include "CTile.h"
#include "CCamera.h"
#include "CTexture.h"

int CTile::g_iTileSize = 64;

CTile::CTile(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_iImageIdx(-1), // -1 Mean : none
	m_iMaxCol(0),
	m_iMaxRow(0)
{
	CTexture* pTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_Tile);
	if (nullptr == pTexture) {
		pTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_Tile, STR_FILE_PATH_Tile);
	}
	assert(pTexture);
	SetTexture(pTexture);

	m_iMaxCol = pTexture->GetWidth() / g_iTileSize;
	m_iMaxRow = pTexture->GetHeight() / g_iTileSize;

	assert((0 != m_iMaxCol) && L"max Col is 0 (tile)");
	assert((0 != m_iMaxRow) && L"max Row is 0 (tile)");
}

CTile::~CTile()
{
}

void CTile::Update()
{
}

void CTile::Render(HDC _hDC)
{
	Vector3 vRenderPos = CCamera::GetInstance()->GetRenderPosition(GetPosition());

	if (-1 == m_iImageIdx)
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
}

void CTile::RenderDefaultTile(HDC _hDC, const Vector3& _vRenderPos)
{
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, hBrush);
	Rectangle(_hDC, (int)_vRenderPos.x, (int)_vRenderPos.y, (int)_vRenderPos.x + GetTileSize(), (int)_vRenderPos.y + GetTileSize());
	SelectObject(_hDC, hOldBrush);
	DeleteObject(hBrush);
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
}

void CTile::Load(FILE* _pFile)
{
	fread(&m_iImageIdx, sizeof(int), 1, _pFile);
}