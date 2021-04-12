#pragma once
#include "CGTA_Vehicle.h"
class CCar : public CGTA_Vehicle
{
public:
	virtual void Update() override;

public:
	CLONE(CCar);
public:
	CCar() = delete;
	CCar(E_GroupType _eGroupType);
	virtual ~CCar() override;
};