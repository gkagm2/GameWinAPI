#pragma once
#include "CObject.h"
class CVehicle : public CObject
{
protected:
    Vector3 m_vPrevHeadDir;

public:

    virtual void Init() override;
    virtual void PrevUpdate() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(HDC _hDC) override;

public:
    CLONE(CVehicle);

public:
    CVehicle() = delete;
    CVehicle(E_GroupType _eGroupType);
    virtual ~CVehicle() override;
};