#include "stdafx.h"
#include "CImageUI.h"
#include "CCamera.h"
#include "CObject.h"
#include "CTexture.h"
// Test
#include "CDebug.h"

const int CImageUI::g_iTileSize = TILE_SIZE;

CImageUI::CImageUI(E_GroupType _eGroupType = E_GroupType::UI) :
	CUI(_eGroupType)
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
    Debug->Print(vFinalPos + 5.0f, L"s", GetObjectName().c_str());

    // ���� Rectangle �׸���
    HPEN hPen = nullptr;

    if (GetTexture()) { // Texture�� ������
        if (m_bIsOn)
            hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // Green color
        else
            hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // Red color

        HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);
        HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);

        Rectangle(_hDC, vFinalPos.x, vFinalPos.y, vFinalPos.x + vScale.x, vFinalPos.y + vScale.y);

        SelectObject(_hDC, oldBrush);
        SelectObject(_hDC, hOldPen);

        DeleteObject(myBrush);
        DeleteObject(hPen);
    }
    else { // Texture�� ������
        CTexture* pTexture = GetTexture();
        BitBlt(_hDC, (int)vFinalPos.x, (int)vFinalPos.y, g_iTileSize, g_iTileSize, pTexture->GetDC(), (int)GetMax().x, (int)GetMax().y, SRCCOPY);
    }
}

void CImageUI::OnPointerClick()
{
}