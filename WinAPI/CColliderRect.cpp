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

    Vector3 leftVec = GetMinPos();
    Vector3 midle = GetPosition();
    leftVec.y = 0;
    Vector3 sc = GetScale();

    float fHalfWidth = GetScale().x * 0.5f;
    float fHalfHeight = GetScale().y * 0.5f;

    // left top
    Vector3 upVec = GetOwnerObject()->GetUpVector();
    Vector3 rightVec = GetOwnerObject()->GetRightVector();

    Vector3 vRTPos = upVec * fHalfHeight + rightVec * fHalfWidth;
    Vector3 vRBPos = -upVec * fHalfHeight + rightVec * fHalfWidth;
    Vector3 vLTPos = upVec * fHalfHeight - rightVec * fHalfWidth;
    Vector3 vLBPos = -upVec * fHalfHeight - rightVec * fHalfWidth;

    vLTPos = MainCamera->GetRenderPosition(GetPosition() + vLTPos);
    vRTPos = MainCamera->GetRenderPosition(GetPosition() + vRTPos);
    vLBPos = MainCamera->GetRenderPosition(GetPosition() + vLBPos);
    vRBPos = MainCamera->GetRenderPosition(GetPosition() + vRBPos);

    MoveToEx(_hDC, vLTPos.x, vLTPos.y, nullptr);
    LineTo(_hDC, vLBPos.x, vLBPos.y);

    MoveToEx(_hDC, vLBPos.x, vLBPos.y, nullptr);
    LineTo(_hDC, vRBPos.x, vRBPos.y);

    MoveToEx(_hDC, vRBPos.x, vRBPos.y, nullptr);
    LineTo(_hDC, vRTPos.x, vRTPos.y);

    MoveToEx(_hDC, vRTPos.x, vRTPos.y, nullptr);
    LineTo(_hDC, vLTPos.x, vLTPos.y);

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