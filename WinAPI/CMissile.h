#pragma once
#include "CObject.h"

enum class E_MissileType {
	NORMAL,
	UPGRADE,
	GUIDED // 유도탄
};

class CCollider;
class CMissile : public CObject
{
private:
	


protected:
	float m_fSpeed;
	E_MissileType m_eMissileType;
	float m_fLivingMaxTime;
	float m_fLivingTime;

	CObject* m_pTargetObject; // 타격할 오브젝트


	// Guided Missile Setting
	float m_testCoolTime;
	float m_testMaxCoolTime;
	Vector3 m_vPrevHeadDir;

private:
	float GetTargetAngle(const Vector3& _vPos, const Vector3& _vTargetPos);

public:
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetDirectionVector(Vector3 _vDirVec) { m_vPrevHeadDir = _vDirVec; }
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