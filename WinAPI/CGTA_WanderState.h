#pragma once
#include "CGTA_AIState.h"

class CGTA_WanderState : public CGTA_AIState
{
private:
	CObject* m_pTarget;
	Vector3 m_vStartPos;
	Vector3 m_vDestPos; // Destination Position

	bool m_bIsPathFind;

	float m_fMinSearchDistance;
	float m_fMaxSearchDistance;

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void Start() override;
	virtual void End() override;

public:
	TTilePos GetRandomDestinationPos(float _fMinSearchDistance, float _fMaxSearchDistance);

	// FIXED : BFS를 이용하여 목적지 랜덤 설정. 사용 안함
	TTilePos GetRandomDestinationPos(int _iDepthMin, int _iDepthMax);

public:
	CLONE(CGTA_WanderState);

public:
	CGTA_WanderState();
	virtual ~CGTA_WanderState() override;
};