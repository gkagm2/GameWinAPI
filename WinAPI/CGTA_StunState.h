#pragma once
#include "CGTA_AIState.h"
class CGTA_StunState : public CGTA_AIState
{
private:
	float m_fStunCoolTime;
	float m_fStunMaxCoolTime;
public:
	virtual void Update() override;

	virtual void Start() override;
	virtual void End() override;

public:
	CLONE(CGTA_StunState);
public:
	CGTA_StunState();
	virtual ~CGTA_StunState() override;
};