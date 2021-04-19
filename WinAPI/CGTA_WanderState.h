#pragma once
#include "CGTA_AIState.h"
class CGTA_WanderState : public CGTA_AIState
{
private:
	CObject* m_pTarget;
public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void Start() override;
	virtual void End() override;

public:
	CGTA_WanderState();
	virtual ~CGTA_WanderState() override;
};