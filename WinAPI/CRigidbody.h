#pragma once
#include "CComponent.h"

class CObject;
class CRigidbody : public CComponent
{
private:
	CObject* m_pOwnerObject;

private:
	Vector3 m_vForce; // ��
	Vector3 m_vAccel; // ���ӵ�
	float m_fMass;	// ����
	Vector3 m_vVelocity; // �ӵ�
	float m_fDrag; // ���� ����

	float m_fSpeed; // �ӷ�
	float m_fMaxSpeed; // �ִ� �ӷ�

	// TOOD : ������ ����� ������ �߰�
	float m_fFriction; // ������
	bool m_bIsFrictionActive; // ������ ����

	Vector3 m_vGracityAccel; // �߷� ���ӵ�
	bool m_bUseGravity; // �߷� ��� �÷���
	bool m_bIsKinematic; // ���� ȿ�� ���ֱ� �÷���

	bool m_bIsActive;

public:
	CObject* GetOwnerObject() { return m_pOwnerObject; }

public:
	virtual void PrevUpdate();
	virtual void Update();
	virtual void LateUpdate();

public:
	void SetMaxSpeed(float _fMaxSpeed) { m_fMaxSpeed = _fMaxSpeed; } // �ִ� �ӵ�
	void SetMass(float _fMass) { m_fMass = max(_fMass, 0.0f); } // ���� (0 ~ alpha)
	float GetMass() const { return m_fMass; }
	void SetVelocity(Vector3 _fVelocity) { m_vVelocity = _fVelocity; }
	const Vector3& GetVelocity() const { return m_vVelocity; }
	void SetDrag(float _fDrag) { m_fDrag = max(_fDrag, 0.0f); } // ���� ���� (0 ~ alpha)
	float GetDrag(float _fDrag) const { return m_fDrag; }
	float GetSpeed() { return m_fSpeed; }
	// inline void SetAngularDrag(float _fAngularDrag); // ȸ������ ���� ����
	void UseGravity(bool _bUseGravity) { m_bUseGravity = _bUseGravity; } // �߷� ��� ���� ����
	bool IsOnGracity() const { return m_bUseGravity; }
	void IsKinematic(bool _bIsKinematic) { m_bIsKinematic = _bIsKinematic; } // ���� ȿ�� ���ֱ�
	bool IsOnKinematic() const { return m_bIsKinematic; }

public:
	void AddForce(Vector3 _vForce) { m_vForce += _vForce; } // ���� �߰��ϴ�.
	void AddVelocity(Vector3 _vVelocity) { m_vVelocity += _vVelocity; } // �ӵ��� �߰��ϴ�.

	const Vector3& GetForce() { return m_vForce; } //read only

public:
	CLONE(CRigidbody);
private:
	CRigidbody(const CRigidbody& _other);

public:
	bool GetActive() { return m_bIsActive; }
	void SetActive(bool _bIsActive) { m_bIsActive = _bIsActive; }

public:
	CRigidbody() = delete;
	CRigidbody(CObject* _pTargetObj);
	virtual ~CRigidbody();

public:
	friend class CObject;
};