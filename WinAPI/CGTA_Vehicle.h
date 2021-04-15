#pragma once
#include "CObject.h"
class CGTA_Vehicle : public CObject
{
public:
    virtual void Init() override;
    virtual void PrevUpdate() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(HDC _hDC) override;

public:
    virtual void Move() {};
    virtual void Shoot() {};
    virtual void Drive() {};
    virtual void Punch() {};
    virtual void Dead() {};
    virtual void Explosion() {};


public:
    CLONE(CGTA_Vehicle);

public:
    CGTA_Vehicle(const CGTA_Vehicle& _origin);

public:
    CGTA_Vehicle() = delete;
    CGTA_Vehicle(E_GroupType _eGroupType);
    virtual ~CGTA_Vehicle() override;
};