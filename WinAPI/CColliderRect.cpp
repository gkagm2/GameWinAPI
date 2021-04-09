#include "stdafx.h"
#include "CColliderRect.h"
#include "CCollider.h"
#include "CCamera.h"
#include "CCore.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CAnimator.h"

#include "CDebug.h"

CColliderRect::CColliderRect(CObject* _pOwnerObject) :
    CCollider(_pOwnerObject),
    m_vOffsetMinPosition{},
    m_vOffsetMaxPosition{},
    m_vScale{ _pOwnerObject->GetScale() }
{
    if (nullptr != _pOwnerObject) {
        if (nullptr != _pOwnerObject->GetTexture()) {
            if (nullptr != _pOwnerObject->GetAnimator()) {
                m_vScale.x = _pOwnerObject->GetAnimator()->GetAnimTexWidth();
                m_vScale.y = _pOwnerObject->GetAnimator()->GetAnimTexHeight();
            }
            else {
                m_vScale.x = _pOwnerObject->GetTextureWidth();
                m_vScale.y = _pOwnerObject->GetTextureHeight();
            }

        }
    }
}

CColliderRect::~CColliderRect()
{
}

void CColliderRect::Render(HDC _hDC)
{
    Vector3 vPosition = GetPosition();
    Vector3 vRenderPosition = MainCamera->GetRenderPosition(vPosition);

    Debug->Print(vRenderPosition, L"dd", vRenderPosition.x, vRenderPosition.y);

    // viewport coordinate
    Vector3 vMin = MainCamera->GetRenderPosition(GetMinPos());
    Vector3 vMax = MainCamera->GetRenderPosition(GetMaxPos());

    HPEN hPen = nullptr;
    if (IsHit())
        hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Red color
    else
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // Green color

    HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

    HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);

    //////////////////////////////////

    

    float disLT = CMyMath::GetDistance(GetPosition(), GetMinPos());
    float disRT = CMyMath::GetDistance(GetPosition(), Vector3(GetMaxPos().x, GetMinPos().y, 0.f));
    float disLB = CMyMath::GetDistance(GetPosition(), Vector3(GetMinPos().x, GetMaxPos().y, 0.f));
    float disRB = CMyMath::GetDistance(GetPosition(), GetMaxPos());

    Vector3 vLTPos{ 0,0,0 };
    Vector3 vLBPos = GetOwnerObject()->GetUpVector();
    vLBPos.y *= 1;
    Vector3 vRBPos{};
    Vector3 vRTPos = Rotate(vLBPos, -90);
    vRBPos.x = vRTPos.x + vLBPos.x - vLTPos.x;
    vRBPos.y = vRTPos.y + vLBPos.y - vLTPos.y;
    Vector2 minVec;
    Vector2 maxVec;

    vLTPos *= disLT;
    vLBPos *= disLB;
    vRBPos *= disRB;
    vRTPos *= disRT;
    minVec.x = min(vLTPos.x, min(vLBPos.x, min(vRBPos.x, vRTPos.x)));
    minVec.y = min(vLTPos.y, min(vLBPos.y, min(vRBPos.y, vRTPos.y)));
    maxVec.x = max(vLTPos.x, max(vLBPos.x, max(vRBPos.x, vRTPos.x)));
    maxVec.y = max(vLTPos.y, max(vLBPos.y, max(vRBPos.y, vRTPos.y)));

    Vector2 middlePos;
    middlePos = (minVec + maxVec) * 0.5f;
    vLTPos = MainCamera->GetRenderPosition(GetPosition() + middlePos - vLTPos);
    vRTPos = MainCamera->GetRenderPosition(GetPosition() + middlePos - vRTPos);
    vLBPos = MainCamera->GetRenderPosition(GetPosition() + middlePos - vLBPos);
    vRBPos = MainCamera->GetRenderPosition(GetPosition() + middlePos - vRBPos);
    

    MoveToEx(_hDC, vLTPos.x, vLTPos.y, nullptr);
    LineTo(_hDC, vLBPos.x, vLBPos.y);

    MoveToEx(_hDC, vLBPos.x, vLBPos.y, nullptr);
    LineTo(_hDC, vRBPos.x, vRBPos.y);

    MoveToEx(_hDC, vRBPos.x, vRBPos.y, nullptr);
    LineTo(_hDC, vRTPos.x, vRTPos.y);

    MoveToEx(_hDC, vRTPos.x, vRTPos.y, nullptr);
    LineTo(_hDC, vLTPos.x, vLTPos.y);

    ///////////////////////////////

    //Rectangle(_hDC, (int)vMin.x, (int)vMin.y, (int)vMax.x, (int)vMax.y);
        
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