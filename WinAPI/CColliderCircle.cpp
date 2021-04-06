#include "stdafx.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CColliderCircle.h"
#include "CCamera.h"
#include "CCore.h"

CColliderCircle::CColliderCircle(CObject* _pOwnerObject) :
    CCollider(_pOwnerObject),
    m_fRadius(3.0f)
{
    Vector3 vGap = (_pOwnerObject->GetMax() - _pOwnerObject->GetMin()) / 2.0f;
    float radius = vGap.x > vGap.y ? vGap.x : vGap.y;
    SetRadius(radius);
}

CColliderCircle::~CColliderCircle()
{
}

void CColliderCircle::Render(HDC _hDC)
{
    Vector3 vPosition = GetPosition();
    Vector3 vRenderPosition = MainCamera->GetRenderPosition(vPosition);
    float radius = m_fRadius;

    // Draw Circle
    HPEN hPen = nullptr;
    if (IsHit())
        hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Red color
    else
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // Green color
    
    HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

    HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);

    Ellipse(_hDC, int(vRenderPosition.x - radius), int(vRenderPosition.y - radius), int(vRenderPosition.x + radius), int(vRenderPosition.y + radius));

    SelectObject(_hDC, oldBrush);
    SelectObject(_hDC, hOldPen);
    DeleteObject(myBrush);
    DeleteObject(hPen);
}

void CColliderCircle::Update()
{
}

void CColliderCircle::LateUpdate()
{
}