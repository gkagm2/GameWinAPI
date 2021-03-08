#include "stdafx.h"
#include "CColliderRect.h"
#include "CCollider.h"
#include "CCamera.h"
#include "CCore.h"

CColliderRect::CColliderRect(CObject* _pOwnerObject) :
    CCollider(_pOwnerObject),
    m_vOffsetMinPosition{},
    m_vOffsetMaxPosition{},
    m_vScale{ _pOwnerObject->GetScale() }
{
}

CColliderRect::~CColliderRect()
{
}

void CColliderRect::Render(HDC _hDC)
{
    Vector3 vPosition = GetPosition();
    Vector3 vRenderPosition = CCamera::GetInstance()->GetRenderPosition(vPosition);

    // viewport coordinate
    Vector3 vMin = CCamera::GetInstance()->GetRenderPosition(GetMinPos());
    Vector3 vMax = CCamera::GetInstance()->GetRenderPosition(GetMaxPos());

    HPEN hPen = nullptr;
    if (IsHit())
        hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Red color
    else
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // Green color

    HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

    HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);

    Rectangle(_hDC, (int)vMin.x, (int)vMin.y, (int)vMax.x, (int)vMax.y);

    SelectObject(_hDC, oldBrush);
    SelectObject(_hDC, hOldPen);
    DeleteObject(myBrush);
    DeleteObject(hPen);
}

void CColliderRect::Update()
{
}

void CColliderRect::LateUpdate()
{
}