#pragma once
#include "CObject.h"
class CVehicle : public CObject
{
    Vector3 m_vDir;
public:
    Vector3 m_vMiddlePos;
public:
    Vector3 pnt[3];

public:
    virtual void Update() override;
    virtual void Render(HDC _hDC) override;


public:
    CLONE(CVehicle);

public:
    CVehicle() = delete;
    CVehicle(E_GroupType _eGroupType);
    virtual ~CVehicle() override;
};