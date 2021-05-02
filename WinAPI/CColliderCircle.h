#pragma once
#include "CCollider.h"
class CColliderCircle : public CCollider
{
private:
    float m_fRadius;

public:
    virtual void Render(HDC _hDC) override;
    virtual void Update() override;
    virtual void LateUpdate() override;

    void SetRadius(float _fRadius) { m_fRadius = _fRadius; }
    float GetRadius() const { return m_fRadius; }
    
public:
    virtual CLONE(CColliderCircle) ; // override

public:
    CColliderCircle(const CColliderCircle& _origin);

public:
    CColliderCircle() = delete;
    CColliderCircle(CObject* _pOwnerObject);
    virtual ~CColliderCircle() override;

public:
    CColliderCircle& operator=(const CColliderCircle& _other) = delete;
};