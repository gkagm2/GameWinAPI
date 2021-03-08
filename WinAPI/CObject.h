#pragma once
#include "stdafx.h"

class CTexture;
class CCollider;
class CObject
{
private:
	Vector3 m_vPosition;
	Vector3 m_vScale;

	CTexture* m_pTexture;	// 텍스쳐
	CCollider* m_pCollider; // 충돌체 컴포넌트

	wstring m_strName;		// 오브젝트 이름
	bool m_bIsDead;			// 오브젝트가 죽었으면 flag true
	bool m_bIsRender;		// 렌더링 여부


private:
	void _SetIsDead(bool _bIsDead) { m_bIsDead = _bIsDead; }

protected:
	E_GroupType m_eGroupType;
	
	void SetGroupType(E_GroupType _eGroupType) { m_eGroupType = _eGroupType; }

public:
	void SetRender(bool _bIsRender) { m_bIsRender = _bIsRender; }
	bool IsRender() { return m_bIsRender; }

	void SetPosition(Vector3 _vPos) { m_vPosition = _vPos; }
	void SetPosition(Vector2 _vPos) { m_vPosition = _vPos; }
	void SetPosition(float _x, float _y, float _z = 0) { m_vPosition.x = _x; m_vPosition.y = _y; m_vPosition.z = _z; }
	Vector3 GetPosition() { return m_vPosition; }

	void SetScale(Vector3 _ptPos) { m_vScale = _ptPos; }
	void SetScale(float _x, float _y, float _z = 1) { m_vScale.x = _x; m_vScale.y = _y; m_vScale.z = _z; }
	Vector3 GetScale() { return m_vScale; }

	Vector3 GetMin();
	Vector3 GetMax();

	void SetTexture(CTexture* _pTexture);
	CTexture* GetTexture() { return  m_pTexture; }

	void SetCollider(CCollider* _pCollider) { m_pCollider = _pCollider;}
	CCollider* GetCollider() { return m_pCollider; }

	E_GroupType GetGroupType() {
		if (this == nullptr)
 			int j = 0;
		int i = 0;
		return m_eGroupType; 
	}

	wstring GetObjectName() { return m_strName; }
	void SetObjectName(const wstring& _strName) { m_strName = _strName; }

	bool IsDead() { return m_bIsDead; }

	inline float PosX() const { return m_vPosition.x; }
	inline float PosY() const { return m_vPosition.y; }
	inline float PosZ() const { return m_vPosition.z; }
	inline float ScaleX() const { return m_vScale.x; }
	inline float ScaleY() const { return m_vScale.y; }
	inline float ScaleZ() const { return m_vScale.z; }

public:
	virtual void Update() = 0;
	virtual void LateUpdate();
	virtual void Render(HDC _hDC) final;

	virtual void OnCollisionEnter(CObject* _pOther){}
	virtual void OnCollisionStay(CObject* _pOther){}
	virtual void OnCollisionExit(CObject* _pOther){}

public:
	CObject() = delete;
	CObject(E_GroupType e_GroupType);
	virtual ~CObject();

public:
	friend class CEventManager;
};

