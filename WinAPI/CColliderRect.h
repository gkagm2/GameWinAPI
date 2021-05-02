#pragma once
#include "CCollider.h"
#include "CObject.h"
class CColliderRect : public CCollider
{
private:
	Vector3 m_vOffsetMinPosition;
	Vector3 m_vOffsetMaxPosition;
	Vector3 m_vScale;			// 충돌체의 범위

public:
	Vector3 GetOffsetMinPos() { return m_vOffsetMinPosition; }
	Vector3 GetOffsetMaxPos() { return m_vOffsetMaxPosition; }
	Vector3 GetScale() { return m_vScale; }
	void SetScale(const Vector3& _vScale) { m_vScale = _vScale;	}
	// viewport coordinate
	Vector3 GetMinPos() { return GetPosition() - m_vScale * 0.5f; }
	Vector3 GetMaxPos() { return GetPosition() + m_vScale * 0.5f; }
	
public:
	virtual void Render(HDC _hDC) override;
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void OnCollisionStay(CCollider* _pOther);

public:
	virtual CLONE(CColliderRect);

public:
	CColliderRect(const CColliderRect& _origin);

public:
	CColliderRect() = delete;
	CColliderRect(CObject* _pOwnerObject);
	virtual ~CColliderRect() override;

public:
	CColliderRect& operator=(const CColliderRect& _other) = delete;
};