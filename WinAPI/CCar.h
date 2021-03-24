#pragma once
#include "CVehicle.h"
class CCar : public CVehicle
{
public:
	virtual void Update() override;
public:
	CCar() = delete;
	CCar(E_GroupType _eGroupType);
	virtual ~CCar() override;
};