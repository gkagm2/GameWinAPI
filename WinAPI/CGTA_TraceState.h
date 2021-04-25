#pragma once
#include "CGTA_AIState.h"
class CGTA_TraceState : public CGTA_AIState
{
private:
	CObject* m_pTarget;
	float m_fFistDistance;
	float m_fDistance;
	float m_fShootDistance;

	bool m_bIsPathFind;
	
public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void Start() override;
	virtual void End() override;

public:
	void SetTarget(CObject* _pTarget) { m_pTarget = _pTarget; }

public:
	CLONE(CGTA_TraceState);

public:
	CGTA_TraceState();
	virtual ~CGTA_TraceState() override;
};