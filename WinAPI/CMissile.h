#pragma once
#include "CObject.h"
class CCollider;
class CMissile :
    public CObject
{
private:
	enum class E_MissileType {
		NORMAL,
		UPGRADE
	};


private:
	float m_fSpeed;
	E_MissileType m_eMissileType;
	float m_fDirDegree;
	
	float m_fLivingMaxTime;
	float m_fLivingTime;
	

public:
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetDirectionDegree(float _fDegree) { m_fDirDegree = _fDegree; }
public:
	virtual void Update() override;

public:
	CMissile() = delete;
	CMissile(E_GroupType _eGroupType);
	virtual ~CMissile() override;
};