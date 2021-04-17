#pragma once
#include "CObject.h"

class CObject;
class CCollider
{
private:
	static UINT g_iColliderID;

protected:
	CObject* m_pOwnerObject;	// 이 객체를 가지고 있는 오브젝트
	int m_iCollisionCount;		// 이 객체가 다른 충돌체와 충돌한 횟수

	Vector3 m_vOffsetPosition;	// 오브젝트 위치로부터 상대적인 차이값
	bool m_bIsActive;			// 활성화 여부

	UINT m_iID;					// 충돌체 고유 식별번호

	bool m_bIsTrigger;			// 충돌 시 고정 여부

public:
	virtual void Render(HDC _hDC) {}
	virtual void Update() {}
	virtual void LateUpdate() {}

	virtual void OnCollisionEnter(CCollider* _pOther) final {
		++m_iCollisionCount;
		m_pOwnerObject->OnCollisionEnter(_pOther->GetOwnerObject());
	}
	virtual void OnCollisionStay(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther) final {
		--m_iCollisionCount;
		assert(!(m_iCollisionCount < 0));
		m_pOwnerObject->OnCollisionExit(_pOther->GetOwnerObject());
	}

public:
	void SetOwnerObject(CObject* _ownerObj) { m_pOwnerObject = _ownerObj; }

	Vector3 GetPosition(); // 충돌체의 최종 위치(Object Position + Collider Position)
	Vector3 GetOffsetPosition() { return m_vOffsetPosition; }
	void SetOffsetPosition(const Vector3& _position) { m_vOffsetPosition += _position; }
	
	CObject* GetOwnerObject() { return m_pOwnerObject; }
	E_GroupType GetGroupType() { return m_pOwnerObject->GetGroupType(); }

	UINT GetID() { return m_iID; }

	void SetActive(bool _bIsActive);
	bool IsActive() { return m_bIsActive; }

	bool IsHit() { return m_iCollisionCount > 0; }

	void SetTrigger(bool _bIsTrigger) { m_bIsTrigger = _bIsTrigger; }
	bool IsTrigger() { return m_bIsTrigger; }
	
public:
	virtual CLONE(CCollider);

public:
	CCollider(const CCollider& _other);

public:
	CCollider() = delete;
	CCollider(CObject* _targetObj);
	
	virtual ~CCollider();

public:
	CCollider& operator=(const CCollider& _other) = delete; // 대인 연산자 제거

public:
	friend class CCollisionManager;
	friend class CObject;
	friend class CEventManager;
};