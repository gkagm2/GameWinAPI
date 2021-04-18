#include "stdafx.h"
#include "CColliderRect.h"
#include "CCollisionManager.h"
#include "CCollider.h"
#include "CCamera.h"
#include "CCore.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CAnimator.h"
#include "CRigidbody.h"

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

CColliderRect::CColliderRect(const CColliderRect& _origin) :
    CCollider(_origin),
    m_vOffsetMinPosition{_origin.m_vOffsetMinPosition},
    m_vOffsetMaxPosition{ _origin.m_vOffsetMaxPosition},
    m_vScale{ _origin.m_vScale }
{
}

CColliderRect::~CColliderRect()
{
}

void CColliderRect::Render(HDC _hDC)
{
    Vector3 vPosition = GetPosition();
    Vector3 vRenderPosition = MainCamera->GetRenderPosition(vPosition);

    // viewport coordinate
    Vector3 vMin = MainCamera->GetRenderPosition(GetMinPos());
    Vector3 vMax = MainCamera->GetRenderPosition(GetMaxPos());
    wstring objName = GetOwnerObject()->GetObjectName();

    if (objName.compare(L"Citizen_copy") == 0) {
        int a = 3;
    }

    HPEN hPen = nullptr;
    if (IsHit())
        hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Red color
    else
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // Green color

    HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

    HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);

    //////////////////////////////////
    /// OBB Rendering
    //Vector3 leftVec = GetMinPos();
    //Vector3 midle = GetPosition();
    //leftVec.y = 0;
    //Vector3 sc = GetScale();

    //float fHalfWidth = GetScale().x * 0.5f;
    //float fHalfHeight = GetScale().y * 0.5f;

    //// left top
    //Vector3 upVec = GetOwnerObject()->GetUpVector();
    //Vector3 rightVec = GetOwnerObject()->GetRightVector();

    //Vector3 vRTPos = upVec * fHalfHeight + rightVec * fHalfWidth;
    //Vector3 vRBPos = -upVec * fHalfHeight + rightVec * fHalfWidth;
    //Vector3 vLTPos = upVec * fHalfHeight - rightVec * fHalfWidth;
    //Vector3 vLBPos = -upVec * fHalfHeight - rightVec * fHalfWidth;

    //vLTPos = MainCamera->GetRenderPosition(GetPosition() + vLTPos);
    //vRTPos = MainCamera->GetRenderPosition(GetPosition() + vRTPos);
    //vLBPos = MainCamera->GetRenderPosition(GetPosition() + vLBPos);
    //vRBPos = MainCamera->GetRenderPosition(GetPosition() + vRBPos);

    //MoveToEx(_hDC, (int)vLTPos.x, (int)vLTPos.y, nullptr);
    //LineTo(_hDC, (int)vLBPos.x, (int)vLBPos.y);

    //MoveToEx(_hDC, (int)vLBPos.x, (int)vLBPos.y, nullptr);
    //LineTo(_hDC, (int)vRBPos.x, (int)vRBPos.y);

    //MoveToEx(_hDC, (int)vRBPos.x, (int)vRBPos.y, nullptr);
    //LineTo(_hDC, (int)vRTPos.x, (int)vRTPos.y);

    //MoveToEx(_hDC, (int)vRTPos.x, (int)vRTPos.y, nullptr);
    //LineTo(_hDC, (int)vLTPos.x, (int)vLTPos.y);

    // AABB Rendering
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

void CColliderRect::OnCollisionStay(CCollider* _pOther)
{
    if (false == _pOther->IsTrigger() && false == m_bIsTrigger) {
        CColliderRect* pOtherColRect = dynamic_cast<CColliderRect*>(_pOther);
        if (nullptr != pOtherColRect) {
            CRigidbody* pOtherRigidbody = pOtherColRect->GetOwnerObject()->GetRigidbody();
            CRigidbody* pThisRigidbody = GetOwnerObject()->GetRigidbody(); 

            // 서로 리지드 바디가 있으면
            if (nullptr != pOtherRigidbody && nullptr != pThisRigidbody) {
                // 질량이 높은것이 Hold, 낮은것이 밀린다.
                if (pOtherRigidbody->GetMass() > pThisRigidbody->GetMass())
                    CCollisionManager::GetInstance()->SetNotIntersection(this, pOtherColRect);
            }
            else if(nullptr != pOtherRigidbody)
                CCollisionManager::GetInstance()->SetNotIntersection(pOtherColRect, this);
        
        }
    }

    CCollider::OnCollisionStay(_pOther);
}
