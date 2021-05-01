#pragma once
#include "CGTA_AIState.h"
class CGTA_RunawayState : public CGTA_AIState
{
private:
	CObject* m_pTarget;
	float m_fMaxRunDistance;

	float m_fPathFindCoolTime;
	float m_fPathFindMaxCoolTime;

	bool m_bIsPathFind;
	float m_fHelpSoundCoolTime;
	float m_fHelpSoundMaxCoolTime;

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void Start() override;
	virtual void End() override;

public:
	CLONE(CGTA_RunawayState);

public:
	CGTA_RunawayState();
	virtual ~CGTA_RunawayState() override;
};