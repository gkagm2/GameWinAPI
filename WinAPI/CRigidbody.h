#pragma once
#include "CComponent.h"

class CObject;
class CRigidbody : public CComponent
{
private:
	CObject* m_pOwnerObject;

private:
	Vector3 m_vForce; // 힘
	Vector3 m_vAccel; // 가속도
	float m_fMass;	// 질량
	Vector3 m_vVelocity; // 속도
	float m_fDrag; // 공기 저항

	float m_fSpeed; // 속력
	float m_fMaxSpeed; // 최대 속력

	// TOOD : 접촉할 경우의 마찰력 추가
	float m_fFriction; // 마찰력
	bool m_bIsFrictionActive; // 마찰력 유무

	Vector3 m_vGracityAccel; // 중력 가속도
	bool m_bUseGravity; // 중력 사용 플래그
	bool m_bIsKinematic; // 물리 효과 없애기 플래그

	bool m_bIsActive;

public:
	CObject* GetOwnerObject() { return m_pOwnerObject; }

public:
	virtual void PrevUpdate();
	virtual void Update();
	virtual void LateUpdate();

public:
	void SetMaxSpeed(float _fMaxSpeed) { m_fMaxSpeed = _fMaxSpeed; } // 최대 속도
	void SetMass(float _fMass) { m_fMass = max(_fMass, 0.0f); } // 질량 (0 ~ alpha)
	float GetMass() const { return m_fMass; }
	void SetVelocity(Vector3 _fVelocity) { m_vVelocity = _fVelocity; }
	const Vector3& GetVelocity() const { return m_vVelocity; }
	void SetDrag(float _fDrag) { m_fDrag = max(_fDrag, 0.0f); } // 공기 저항 (0 ~ alpha)
	float GetDrag(float _fDrag) const { return m_fDrag; }
	float GetSpeed() { return m_fSpeed; }
	// inline void SetAngularDrag(float _fAngularDrag); // 회전값에 대한 저항
	void UseGravity(bool _bUseGravity) { m_bUseGravity = _bUseGravity; } // 중력 사용 여부 설정
	bool IsOnGracity() const { return m_bUseGravity; }
	void IsKinematic(bool _bIsKinematic) { m_bIsKinematic = _bIsKinematic; } // 물리 효과 없애기
	bool IsOnKinematic() const { return m_bIsKinematic; }

public:
	void AddForce(Vector3 _vForce) { m_vForce += _vForce; } // 힘을 추가하다.
	void AddVelocity(Vector3 _vVelocity) { m_vVelocity += _vVelocity; } // 속도를 추가하다.

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