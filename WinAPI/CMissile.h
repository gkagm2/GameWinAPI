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
	float m_fDirAngle;
	
	float m_fLivingMaxTime;
	float m_fLivingTime;
	

public:
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetDirectionAngle(float _fDirAngle) { m_fDirAngle = _fDirAngle; }
public:
	virtual void Update() override;

public:
	CMissile() = delete;
	CMissile(E_GroupType _eGroupType);
	virtual ~CMissile() override;
};