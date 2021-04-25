#pragma once
#include "CGTA_AIState.h"
class CGTA_DeadState : public CGTA_AIState
{
private:
	float m_fDeadCoolTime;
	float m_fDeadMaxCoolTime;
public:
	virtual void Update() override;
	virtual void Start() override;
	virtual void End() override;

public:
	CLONE(CGTA_DeadState);
public:
	CGTA_DeadState();
	virtual ~CGTA_DeadState() override;
};