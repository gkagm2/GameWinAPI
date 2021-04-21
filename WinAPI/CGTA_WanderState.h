#pragma once
#include "CGTA_AIState.h"

class CGTA_WanderState : public CGTA_AIState
{
private:
	CObject* m_pTarget;
	Vector3 m_vStartPos;
	Vector3 m_vDestPos; // Destination Position

	float m_fPathFindCoolTime;
	float m_fPathFindMaxCoolTime;
	bool m_bIsPathFind;

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void Start() override;
	virtual void End() override;

public:
	CLONE(CGTA_WanderState);

public:
	CGTA_WanderState();
	virtual ~CGTA_WanderState() override;
};