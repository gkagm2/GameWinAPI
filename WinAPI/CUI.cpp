#include "stdafx.h"
#include "CUI.h"
#include "CCore.h"

CUI::CUI(E_GroupType _eGroupType = E_GroupType::UI) :
	CObject(_eGroupType),
    m_pParentUI(nullptr),
    m_ePivotState(E_UIPivot::leftTop),
    m_vPivotPosition(GetPosition())
{
}

CUI::~CUI()
{
}

void CUI::Update()
{
}

void CUI::LateUpdate()
{
}

void CUI::Render(HDC _hDC)
{
    // 투명 Rectangle 그리기
    HPEN hPen = hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // Green color
    HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);
    HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);

	Ellipse(_hDC, 100, 100, 120, 120);

	SelectObject(_hDC, oldBrush);
    SelectObject(_hDC, hOldPen);

	DeleteObject(myBrush);
    DeleteObject(hPen);
}
// TODO : Min, Max 구하기 구현
/*
Vector3 CUI::GetMin()
{
    switch (m_ePivotState) {
    case E_UIPivot::leftTop:
        break;
    case E_UIPivot::left:
        break;
    case E_UIPivot::leftBottom:
        break;
    case E_UIPivot::top:
        break;
    case E_UIPivot::center:
        break;
    case E_UIPivot::rightTop:
        break;
    case E_UIPivot::right:
        break;
    case E_UIPivot::rightBottom:
        break;
    }
    return Vector3();
}
Vector3 CUI::GetMax()
{
    switch (m_ePivotState) {
    case E_UIPivot::leftTop:
        break;
    case E_UIPivot::left:
        break;
    case E_UIPivot::leftBottom:
        break;
    case E_UIPivot::top:
        break;
    case E_UIPivot::center:
        break;
    case E_UIPivot::rightTop:
        break;
    case E_UIPivot::right:
        break;
    case E_UIPivot::rightBottom:
        break;
    }
    return Vector3();
}
*/