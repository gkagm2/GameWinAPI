#include "stdafx.h"
#include "CTerrain.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CTexture.h"
#include "CCamera2D.h"

CTerrain::CTerrain(E_GroupType _eGroupType) :
	CObject(E_GroupType::TERRAIN)
{
}

CTerrain::~CTerrain()
{
}

void CTerrain::Init()
{
	CTexture* pTexture = CResourceManager::GetInstance()->GetTexture(STR_FILE_PATH_Terrain, STR_FILE_PATH_Terrain);
	SetTexture(pTexture);

	// Test 텍스쳐 색상 칠해보기
	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x) {
			pTexture->SetPixel(x, y, TRGB(0, 255, 0));
		}
	}
}

void CTerrain::Update()
{
}

void CTerrain::Render(HDC _hDC)
{
	if (nullptr == GetTexture())
		return;

	Vector2 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	BitBlt(_hDC, vRenderPosition.x, vRenderPosition.y, GetTexture()->GetWidth(), GetTexture()->GetHeight(), GetTexture()->GetDC(), 0, 0, SRCCOPY);
}