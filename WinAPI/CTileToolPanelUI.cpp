#include "stdafx.h"
#include "CTileToolPanelUI.h"

CTileToolPanelUI::CTileToolPanelUI(E_GroupType _eGroupType) :
	CPanelUI(_eGroupType),
	m_iSelectedTileIdx(-1), // -1 : NONE
	m_eSelectedTileType(E_TileType::None),
	m_pTileTexture(nullptr)
{
}

CTileToolPanelUI::~CTileToolPanelUI()
{
}

void CTileToolPanelUI::Init()
{
	__super::Init();
}

void CTileToolPanelUI::Update()
{
	__super::Update();
}

void CTileToolPanelUI::LateUpdate()
{
	__super::LateUpdate();
}

void CTileToolPanelUI::Render(HDC _hDC)
{
	__super::Render(_hDC);
}

void CTileToolPanelUI::OnPointerDown()
{
	__super::OnPointerDown();

}

void CTileToolPanelUI::OnPointerUp()
{
	__super::OnPointerUp();
}