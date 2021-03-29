#pragma once
#include "CObject.h"
class CVehicle : public CObject
{
public:
    virtual void Update() override;

public:
    CLONE(CVehicle);

public:
    CVehicle() = delete;
    CVehicle(E_GroupType _eGroupType);
    virtual ~CVehicle() override;
};