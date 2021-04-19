#pragma once
#include "CGTA_AIState.h"
class CGTA_RunawayState : public CGTA_AIState
{
private:
	CObject* m_pTarget;

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void Start() override;
	virtual void End() override;

public:
	CGTA_RunawayState();
	virtual ~CGTA_RunawayState() override;
};