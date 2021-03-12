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
	float m_fDirDegree;
	
	float m_fLivingMaxTime;
	float m_fLivingTime;

	CObject* m_pTargetObject; // 타격할 오브젝트


public:
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetDirectionDegree(float _fDegree) { m_fDirDegree = _fDegree; }
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