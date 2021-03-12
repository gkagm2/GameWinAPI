#pragma once
#include "CObject.h"

enum class E_MissileType {
	NORMAL,
	UPGRADE,
	GUIDED // ����ź
};

class CCollider;
class CMissile : public CObject
{
private:
	


protected:
	float m_fSpeed;
	E_MissileType m_eMissileType;
	float m_fDirDegree;
	
	float m_fLivingMaxTime;
	float m_fLivingTime;

	CObject* m_pTargetObject; // Ÿ���� ������Ʈ


	// Guided Missile Setting
	float m_testCoolTime;
	float m_testMaxCoolTime;
	float m_fPrevRadian;

private:
	float GetTargetAngle(const Vector3& _vPos, const Vector3& _vTargetPos);

public:
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetDirectionDegree(float _fDegree) { 
		m_fDirDegree = _fDegree;
		m_fPrevRadian = m_fDirDegree * CMyMath::Deg2Rad();
	}
	void SetMissileType(E_MissileType _eMissileType);

public:
	void SetTargetObject(CObject* _pTargetObject);

public:
	virtual void Update() override;

public:
	CMissile() = delete;
	CMissile(E_GroupType _eGroupType);
	virtual ~CMissile() override;
};