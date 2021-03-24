#include "stdafx.h"
#include "CUI.h"
#include "CCore.h"
#include "CCamera.h"
#include "CTexture.h"

//Test
#include "CCore.h"
#include "CDebug.h"

CUI::CUI(E_GroupType _eGroupType = E_GroupType::UI) :
    CObject(_eGroupType),
    m_pParentUI(nullptr),
    m_ePivotState(E_UIPivot::leftTop),
    m_vFinalPosition{},
    m_bIsOn(false)
{
}

CUI::~CUI()
{
    for (UINT i = 0; i < m_vecChildUI.size(); ++i) {
        if (nullptr != m_vecChildUI[i])
            delete m_vecChildUI[i];
    }
}

void CUI::Update()
{
    for (UINT i = 0; i < m_vecChildUI.size(); ++i)
        m_vecChildUI[i]->Update();
}

void CUI::LateUpdate()
{
    if (m_pParentUI)
        m_vFinalPosition =GetPosition() + m_pParentUI->GetFinalPosition();
    else
        m_vFinalPosition = GetPosition();

    for (UINT i = 0; i < m_vecChildUI.size(); ++i)
        m_vecChildUI[i]->LateUpdate();
}

void CUI::Render(HDC _hDC)
{
    Vector3 vPosition = GetPosition();
    Vector3 vScale = GetScale();

    Debug->Print(vPosition + 5.0f, L"s", GetObjectName().c_str());

    // 투명 Rectangle 그리기
    HPEN hPen = nullptr;
    if (m_bIsOn)
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // Green color
    else
        hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // Red color
    HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);
    HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);

    Rectangle(_hDC, vPosition.x, vPosition.y, vPosition.x + vScale.x, vPosition.y + vScale.y);

	SelectObject(_hDC, oldBrush);
    SelectObject(_hDC, hOldPen);

	DeleteObject(myBrush);
    DeleteObject(hPen);

    for (UINT i = 0; i < m_vecChildUI.size(); ++i)
        m_vecChildUI[i]->Render(_hDC);
}
void CUI::OnPointerEnter()
{
}
void CUI::OnPointerExit()
{
}
void CUI::OnPointerDown()
{
}
void CUI::OnPointerUp()
{
}
void CUI::OnPointerClick()
{
}

bool CUI::IsPointerOn(const Vector2& _vMousePosition)
{
    Vector2 mousePos = _vMousePosition;
    Vector3 minPos = GetMin();
    Vector3 maxPos = GetMax();
    if (_vMousePosition.x >= GetMin().x && _vMousePosition.x <= GetMax().x &&
        _vMousePosition.y >= GetMin().y && _vMousePosition.y <= GetMax().y) {
        return true;
    }
    return false;
}

// TODO : Min, Max 구하기 구현
Vector3 CUI::GetMin()
{
    switch (m_ePivotState) {
    case E_UIPivot::leftTop:
    {
        Vector3 minVec = GetPosition();
        CTexture* pTexture = GetTexture();
        if (nullptr != pTexture) {
            minVec.x -= pTexture->GetWidth();
            minVec.y -= pTexture->GetHeight();
        }
        return minVec;
    }
        break;
    case E_UIPivot::left:
        break;
    case E_UIPivot::leftBottom:
        break;
    case E_UIPivot::top:
        break;
    case E_UIPivot::center:
    {
        Vector3 minVec = GetPosition();
        CTexture* pTexture = GetTexture();
        if (nullptr != pTexture) {
            minVec.x -= pTexture->GetWidth() / 2.0f;
            minVec.y -= pTexture->GetHeight() / 2.0f;
        }
        return minVec;
    }
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
    {
        Vector3 maxVec = GetPosition();
        CTexture* pTexture = GetTexture();
        if (nullptr != pTexture) {
            maxVec.x += pTexture->GetWidth();
            maxVec.y += pTexture->GetHeight();
        }
        else
            maxVec += GetScale();
        return maxVec;
    }
    case E_UIPivot::left:
        break;
    case E_UIPivot::leftBottom:
        break;
    case E_UIPivot::top:
        break;
    case E_UIPivot::center:
    {
        Vector3 maxVec = GetPosition();
        CTexture* pTexture = GetTexture();
        if (nullptr != pTexture) {
            maxVec.x += pTexture->GetWidth() / 2.0f;
            maxVec.y += pTexture->GetHeight() / 2.0f;
        }
        else
            maxVec += GetScale() / 2.0f;
        return maxVec;
    }
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