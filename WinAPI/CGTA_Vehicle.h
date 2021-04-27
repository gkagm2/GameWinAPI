#pragma once
#include "CObject.h"
class CGTA_Character;
class CGTA_Vehicle : public CObject
{
private:
    float m_fPower;

    CObject* m_pDriver;
    bool m_bExplosion; // Æø¹ß ¿©ºÎ
public:
    virtual void Init() override;
    virtual void PrevUpdate() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(HDC _hDC) override;

    virtual void OnCollisionEnter(CObject* _pOther) override;
    //virtual void OnCollisionStay(CObject* _pOther) override;
    // virtual void OnCollisionExit(CObject* _pOther) override;

public:
    virtual void DriveUpdate();
    virtual void Move() {};
    virtual void Shoot() {};
    virtual void Drive() {};
    virtual void Punch() {};
    virtual void Dead() {};
    virtual void Explosion() {};

    bool DidExplode() { return m_bExplosion; }

public:
    CObject* GetDriver() { return m_pDriver; }
    void SetDriver(CObject* _pDriver) { m_pDriver = _pDriver; }

public:
    CLONE(CGTA_Vehicle);

public:
    CGTA_Vehicle(const CGTA_Vehicle& _origin);

public:
    CGTA_Vehicle() = delete;
    CGTA_Vehicle(E_GroupType _eGroupType);
    virtual ~CGTA_Vehicle() override;
};