#pragma once
#include "stdafx.h"

class CTexture;
class CCollider;
class CAnimator;
class CRigidbody;

class CObject
{
private:
	Vector3 m_vPosition;
	Vector3 m_vScale;
	// TOOD : Rotation을 넣어야 될 듯
	// Rotate Info
	Vector3 m_vRectPoint[3];
	Vector3 m_vRPDir[3]; //RP : RectPoint
	Vector3 m_vUpVec; // Up vector

	CTexture* m_pTexture;		// 텍스쳐
	CCollider* m_pCollider;		// 충돌체 컴포넌트
	CAnimator* m_pAnimator;		// 애니메이터
	CRigidbody* m_pRigidbody;	// 리지드바디

	wstring m_strName;			// 오브젝트 이름
	bool m_bIsDead;				// 오브젝트가 죽었으면 flag true
	bool m_bIsRender;			// 렌더링 여부
	bool m_bIsActive;			// 활성화 여부


private:
	void _SetIsDead(bool _bIsDead) { m_bIsDead = _bIsDead; }

protected:
	E_GroupType m_eGroupType;
	
	void SetGroupType(E_GroupType _eGroupType) { m_eGroupType = _eGroupType; }

public:
	virtual void Init();
	virtual void PrevUpdate();
	virtual void Update() = 0;
	virtual void LateUpdate();
	virtual void Render(HDC _hDC);

	virtual void OnCollisionEnter(CObject* _pOther) {}
	virtual void OnCollisionStay(CObject* _pOther) {}
	virtual void OnCollisionExit(CObject* _pOther) {}

public:
	void SetRender(bool _bIsRender) { m_bIsRender = _bIsRender; }
	bool IsRender() { return m_bIsRender; }


	// Rotate
	void InitRectPoint();
	const Vector3& GetRectPoint(int _idx) { return m_vRectPoint[_idx]; }
	void SetRectPoint(int _idx, Vector3 _vPoint) { m_vRectPoint[_idx] = _vPoint; }
	// Rotate idx range : 0 ~ 2
	void RotateRP(float _fDegree);

	void SetPosition(Vector3 _vPos) { m_vPosition = _vPos; }
	void SetPosition(Vector2 _vPos) { m_vPosition = _vPos; }
	void SetPosition(float _x, float _y, float _z = 0) { m_vPosition.x = _x; m_vPosition.y = _y; m_vPosition.z = _z; }
	virtual Vector3 GetPosition() { return m_vPosition; }

	void SetScale(Vector3 _ptPos) { m_vScale = _ptPos; }
	void SetScale(float _x, float _y, float _z = 1) { m_vScale.x = _x; m_vScale.y = _y; m_vScale.z = _z; }
	Vector3 GetScale() { return m_vScale; }

	virtual Vector3 GetMin();
	virtual Vector3 GetMax();

	Vector3 GetUpVector() { return m_vUpVec; }

	void SetTexture(CTexture* _pTexture);
	CTexture* GetTexture() { return  m_pTexture; }

	float GetTextureWidth();
	float GetTextureHeight();

	void SetCollider(CCollider* _pCollider) { m_pCollider = _pCollider;}
	void SetAnimator(CAnimator* _pAnimator) { m_pAnimator = _pAnimator; }
	void SetRigidbody(CRigidbody* _pRigidbody) { m_pRigidbody = _pRigidbody; }
	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	CRigidbody* GetRigidbody() { return m_pRigidbody; }

	E_GroupType GetGroupType() {
		return m_eGroupType; 
	}

	const wstring& GetObjectName() { return m_strName; }
	void SetObjectName(const wstring& _strName) { m_strName = _strName; }

	bool IsDead() { return m_bIsDead; }

	bool IsActive() { return m_bIsActive; }
	bool SetActive(bool _bActive) { return m_bIsActive = _bActive; }

	float PosX() const { return m_vPosition.x; }
	float PosY() const { return m_vPosition.y; }
	float PosZ() const { return m_vPosition.z; }
	float ScaleX() const { return m_vScale.x; }
	float ScaleY() const { return m_vScale.y; }
	float ScaleZ() const { return m_vScale.z; }

public:
	void Save();
	void Load();

public:
	virtual CObject* Clone() = 0;

protected:
	CObject(const CObject& _origin);

public:
	CObject() = delete;
	CObject(E_GroupType e_GroupType);
	virtual ~CObject();

public:
	friend class CEventManager;
};