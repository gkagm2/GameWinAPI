#pragma once
#include "CGTA_AIState.h"
class CGTA_IdleState : public CGTA_AIState
{
private:
	CObject* m_pTarget;

public:
	virtual void Update() override;
	virtual void LateUpdate() {};

	virtual void Start() override;
	virtual void End() override;

public:
	CLONE(CGTA_IdleState);
public:
	CGTA_IdleState();
	virtual ~CGTA_IdleState() override;
};