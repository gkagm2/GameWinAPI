#pragma once
#include "CGTA_AIState.h"
class CGTA_IdleState_Citizen : public CGTA_AIState
{
private:
	CObject* m_pTarget;

public:
	virtual void Update() override;
	virtual void LateUpdate() {};

	virtual void Start() override;
	virtual void End() override;

public:
	CGTA_IdleState_Citizen();
	virtual ~CGTA_IdleState_Citizen() override;
};