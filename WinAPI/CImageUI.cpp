#include "stdafx.h"
#include "CImageUI.h"
#include "CCamera.h"
#include "CObject.h"
#include "CTexture.h"
#include "CTileToolPanelUI.h"
// Test
#include "CDebug.h"

const int CImageUI::g_iTileSize = TILE_SIZE;

CImageUI::CImageUI(E_GroupType _eGroupType = E_GroupType::UI) :
    CUI(_eGroupType),
    m_vLT{},
    m_iTileIdx(-1),
    m_eTileType(E_TileType::None)
{
	m_ePivotState = E_UIPivot::leftTop;
}

CImageUI::~CImageUI()
{
}

void CImageUI::Init()
{
}

void CImageUI::Update()
{
	CUI::Update();
}

void CImageUI::LateUpdate()
{
	CUI::LateUpdate();
}

void CImageUI::Render(HDC _hDC)
{
	Vector3 vFinalPos = GetFinalPosition();
    Vector3 vScale = GetScale();
    Debug->Print(vFinalPos + 30.0f, L"sddi", GetObjectName().c_str(), vFinalPos.x, vFinalPos.y, m_bIsOn);

    // 투명 Rectangle 그리기
    HPEN hPen = nullptr;

    if (nullptr == GetTexture()) { // Texture가 없으면
        if (m_bIsOn)
            hPen = CreatePen(PS_SOLID, 1, RGB(0, 200, 0)); // Green color
        else
            hPen = CreatePen(PS_SOLID, 1, RGB(200, 0, 0)); // Red color

        HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);
        HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);

        Rectangle(_hDC, (int)vFinalPos.x, (int)vFinalPos.y, int(vFinalPos.x + vScale.x), int(vFinalPos.y + vScale.y));

        SelectObject(_hDC, oldBrush);
        SelectObject(_hDC, hOldPen);

        DeleteObject(myBrush);
        DeleteObject(hPen);
    }
    else { // Texture가 있으면
        CTexture* pTexture = GetTexture();
        StretchBlt(_hDC, (int)vFinalPos.x, (int)vFinalPos.y, (int)GetScale().x, (int)GetScale().y, pTexture->GetDC(), (int)m_vLT.x, (int)m_vLT.y, (int)TILE_SIZE, (int)TILE_SIZE, SRCCOPY);
        //TransparentBlt(_hDC, (int)vFinalPos.x, (int)vFinalPos.y, (int)GetScale().x, (int)GetScale().y, pTexture->GetDC(), (int)m_vLT.x, (int)m_vLT.y, (int)TILE_SIZE, (int)TILE_SIZE, 0);
        //BitBlt(_hDC, (int)vFinalPos.x, (int)vFinalPos.y, g_iTileSize, g_iTileSize, pTexture->GetDC(), 1,1, SRCCOPY);
    }

    const vector<CUI*>& vecChildUI = GetChildsUI();
    for (UINT i = 0; i < vecChildUI.size(); ++i)
        vecChildUI[i]->Render(_hDC);
}

void CImageUI::OnPointerClick()
{
    CTileToolPanelUI* pUI = dynamic_cast<CTileToolPanelUI*>(m_pParentUI);
    pUI->SetSelectedTile(m_iTileIdx, m_eTileType, GetTexture(), m_strTexturePath);
}