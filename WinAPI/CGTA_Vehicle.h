#pragma once
#include "CObject.h"

enum class E_VehicleState {
    normal, // 멀쩡합니다.
    explode,// 터졌습니다.
}


struct TVehicleInfo {
    float fHp;
    float fMoveSpeed;
    float fPower;

    TVehicleInfo() : fHp(10.f), fMoveSpeed(60.f), fPower(3000.F){}
    void Save(FILE* _pFile);
    void Load(FILE* _pFile);
};

class CGTA_Character;
class CGTA_Vehicle : public CObject
{
private:
    TVehicleInfo m_tVehicleInfo;

    CObject* m_pDriver;
    bool m_bExplosion; // 폭발 여부
    E_VehicleState m_eVehicleState;
    

public:
    virtual void Init() override;
    virtual void PrevUpdate() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(HDC _hDC) override;

    virtual void OnCollisionEnter(CObject* _pOther) override;
    virtual void OnCollisionStay(CObject* _pOther) override;
    // virtual void OnCollisionExit(CObject* _pOther) override;

public:
    virtual void DriveUpdate();
    virtual void Move() {};
    virtual void Shoot() {};
    virtual void Drive() {};
    virtual void Punch() {};
    virtual void Dead() {};
    virtual void Explosion();

    virtual void State();

    bool DidExplode() { return m_bExplosion; }

public:
    CObject* GetDriver() { return m_pDriver; }
    void SetDriver(CObject* _pDriver) { m_pDriver = _pDriver; }

    TVehicleInfo& VehicleInfo() { return m_tVehicleInfo; }

    E_VehicleState GetVehicleState() { return m_eVehicleState; }
    void SetVehicleState(E_VehicleState _eVehicleState) { m_eVehicleState = _eVehicleState; }

    bool DidExploded() { return m_bExplosion; }

public:
    CLONE(CGTA_Vehicle);

public:
    CGTA_Vehicle(const CGTA_Vehicle& _origin);

public:
    CGTA_Vehicle() = delete;
    CGTA_Vehicle(E_GroupType _eGroupType);
    virtual ~CGTA_Vehicle() override;
};