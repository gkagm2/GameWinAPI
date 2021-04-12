#include "stdafx.h"
#include "CUI.h"
#include "CCore.h"
#include "CCamera.h"
#include "CTexture.h"

#include "CUIManager.h"

//Test
#include "CCore.h"
#include "CDebug.h"

CUI::CUI(E_GroupType _eGroupType = E_GroupType::UI) :
    CObject(_eGroupType),
    m_pParentUI(nullptr),
    m_bIsOn(false),
    m_bIsDown(false),
    m_ePivotState(E_UIPivot::leftTop),
    m_vFinalPosition{}
{
}

CUI::CUI(const CUI& _origin) :
    CObject(_origin),
    m_pParentUI(nullptr),
    m_bIsOn(false),
    m_bIsDown(false),
    m_ePivotState(_origin.m_ePivotState)
{
    for (UINT i = 0; i < _origin.m_vecChildUI.size(); ++i)
        AddChildUI(_origin.m_vecChildUI[i]->Clone());
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
    Vector3 vPosition = GetFinalPosition();
    Vector3 vScale = GetScale();

    Debug->Print(vPosition + 30.0f, L"sdd", GetObjectName().c_str(), vPosition.x, vPosition.y, m_bIsOn);

    // 투명 Rectangle 그리기
    HPEN hPen = nullptr;
    if (m_bIsOn)
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // Green color
    else
        hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // Red color
    HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);
    HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);

    Rectangle(_hDC, (int)vPosition.x, (int)vPosition.y, int(vPosition.x + vScale.x), int(vPosition.y + vScale.y));

	SelectObject(_hDC, oldBrush);
    SelectObject(_hDC, hOldPen);

	DeleteObject(myBrush);
    DeleteObject(hPen);

    for (UINT i = 0; i < m_vecChildUI.size(); ++i)
        m_vecChildUI[i]->Render(_hDC);
}

void CUI::OnPointerDown()
{
    Debug->Print(Vector2(20, 200), L"s", L"OnPointerDown");
}
void CUI::OnPointerUp()
{
    Debug->Print(Vector2(20, 200), L"s", L"OnPointerUp");
}
void CUI::OnPointerClick()
{
    Debug->Print(Vector2(20, 200), L"s", L"OnPointerClick");
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
        Vector3 minVec = GetFinalPosition();
        CTexture* pTexture = GetTexture();
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
        Vector3 minVec = GetFinalPosition();
        CTexture* pTexture = GetTexture();
        if (nullptr != pTexture) {
            /*minVec.x -= pTexture->GetWidth() * 0.5f;
            minVec.y -= pTexture->GetHeight() * 0.5f;*/
            minVec.x -= GetScale().x * 0.5f;
            minVec.y -= GetScale().y * 0.5f;
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
        Vector3 maxVec = GetFinalPosition();
        CTexture* pTexture = GetTexture();
        if (nullptr != pTexture) {
            //maxVec.x += pTexture->GetWidth();
            //maxVec.y += pTexture->GetHeight();
            maxVec.x += GetScale().x;
            maxVec.y += GetScale().y;
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
        Vector3 maxVec = GetFinalPosition();
        CTexture* pTexture = GetTexture();
        if (nullptr != pTexture) {
            /*maxVec.x += pTexture->GetWidth() * 0.5f;
            maxVec.y += pTexture->GetHeight() * 0.5f;*/
            maxVec.x += GetScale().x * 0.5f;
            maxVec.y += GetScale().y * 0.5f;
        }
        else
            maxVec += GetScale() * 0.5f;
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