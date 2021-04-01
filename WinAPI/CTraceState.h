#pragma once
#include "CAIState.h"
class CTraceState : public CAIState
{
private:
	CObject* m_pTarget;

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void Start() override;
	virtual void End() override;
public:
	CTraceState();
	virtual ~CTraceState() override;
};