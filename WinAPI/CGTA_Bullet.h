#pragma once
#include "CObject.h"
class CGTA_Bullet : public CObject
{
private:
	float m_fLifeCoolTime;
	float m_fLifeMaxCoolTime;
	float m_fSpeed;

public:
	virtual void Init() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(CObject* _pOther) override;
	virtual void OnCollisionStay(CObject* _pOther) override;
	virtual void OnCollisionExit(CObject* _pOther) override;

public:
	CLONE(CGTA_Bullet);

	CGTA_Bullet(const CGTA_Bullet& _origin);
public:
	CGTA_Bullet() = delete;
	CGTA_Bullet(E_GroupType e_GroupType);
	virtual ~CGTA_Bullet();
	
};