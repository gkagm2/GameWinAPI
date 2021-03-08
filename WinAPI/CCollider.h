#pragma once
#include "CObject.h"
class CCollider
{
private:
	static UINT g_iColliderID;

protected:
	CObject* m_pOwnerObject;	// �� ��ü�� ������ �ִ� ������Ʈ
	int m_iCollisionCount;		// �� ��ü�� �ٸ� �浹ü�� �浹�� Ƚ��

	Vector3 m_vOffsetPosition;	// ������Ʈ ��ġ�κ��� ������� ���̰�
	bool m_bIsActive;			// Ȱ��ȭ ����

	UINT m_iID;				// �浹ü ���� �ĺ���ȣ

public:
	Vector3 GetPosition(); // �浹ü�� ���� ��ġ(Object Position + Collider Position)
	Vector3 GetOffsetPosition() { return m_vOffsetPosition; }
	void SetOffsetPosition(const Vector3& _position) { m_vOffsetPosition += _position; }
	
	CObject* GetOwnerObject() { return m_pOwnerObject; }
	E_GroupType GetGroupType() { 
		if (m_pOwnerObject == nullptr)
			int i = 0;
		return m_pOwnerObject->GetGroupType(); }

	UINT GetID() { return m_iID; }

	void SetActive(bool _bIsActive) { m_bIsActive = _bIsActive; }
	bool IsActive() { return m_bIsActive; }

	bool IsHit() { return m_iCollisionCount > 0; }
	

public:
	virtual void Render(HDC _hDC) {}
	virtual void Update() {}
	virtual void LateUpdate() {}

	virtual void OnCollisionEnter(CCollider* _pOther) final { 
		++m_iCollisionCount;
		m_pOwnerObject->OnCollisionEnter(_pOther->GetOwnerObject()); }
	virtual void OnCollisionStay(CCollider* _pOther) final { 
		m_pOwnerObject->OnCollisionStay(_pOther->GetOwnerObject()); }
	virtual void OnCollisionExit(CCollider* _pOther) final {
		--m_iCollisionCount;
		assert(!(m_iCollisionCount < 0));
		m_pOwnerObject->OnCollisionExit(_pOther->GetOwnerObject());
	}

public:
	CCollider() = delete;
	CCollider(CObject* _targetObj);
	virtual ~CCollider();

public:
	CCollider& operator=(const CCollider& _other) = delete; // ���� ������ ����

public:
	friend class CCollisionManager;
};