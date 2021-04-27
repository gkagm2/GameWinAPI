#pragma once
#include "CGTA_AIState.h"
class CGTA_Vehicle;
class CGTA_WalkToVehicleState : public CGTA_AIState
{
private :
	CGTA_Vehicle* m_pVehicle;
	bool m_bIsFindPath;

public:
	virtual void Update() override;

	virtual void Start() override;
	virtual void End() override;

public:
	CLONE(CGTA_WalkToVehicleState);
public:
	CGTA_WalkToVehicleState();
	virtual ~CGTA_WalkToVehicleState() override;
};