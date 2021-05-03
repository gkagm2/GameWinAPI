#pragma once
#include "CObject.h"

enum class E_VehicleState {
    normal, // 멀쩡합니다.
    explode,// 터졌습니다.
};

struct TVehicleInfo {
    float fHp;
    float fMoveSpeed;
    float fPower;

    TVehicleInfo() : fHp(150.f), fMoveSpeed(60.f), fPower(120.f){}
    void Save(FILE* _pFile);
    void Load(FILE* _pFile);
};

class CGTA_Character;
class CSound;
class CGTA_Vehicle : public CObject
{
private:
    TVehicleInfo m_tVehicleInfo;

    CObject* m_pDriver;
    bool m_bExplosion; // 폭발 여부
    E_VehicleState m_eVehicleState;

    bool m_bReverse; // 후진 여부
    CSound* m_pEngineSound;

    CSound* m_pRadio;
    CTexture* m_pExplosionTex;

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
    void InitEngineSound(const wstring& _strSoundKey);
    void InitRadio(int _iTypeIdx);

    bool DidExplode() { return m_bExplosion; }
public:
    CObject* GetDriver() { return m_pDriver; }
    void SetDriver(CObject* _pDriver);

    TVehicleInfo& VehicleInfo() { return m_tVehicleInfo; }

    E_VehicleState GetVehicleState() { return m_eVehicleState; }
    void SetVehicleState(E_VehicleState _eVehicleState) { m_eVehicleState = _eVehicleState; }

    bool DidExploded() { return m_bExplosion; }
    bool IsReverse() { return m_bReverse; } // 후진하고있는가?

    void SetEngineSound(CSound* _pEngineSound) { m_pEngineSound = _pEngineSound; }
    CSound* GetEngineSound() { return m_pEngineSound; }

    void SetRadio(CSound* _pRadio) { m_pRadio = _pRadio; }
    CSound* GetRadio() { return m_pRadio; }

    void SetExplosionTex(const wstring& _strTexureKey);
    CTexture* GetExplosionTex() { return m_pExplosionTex; }

public:
    CLONE(CGTA_Vehicle);

public:
    CGTA_Vehicle(const CGTA_Vehicle& _origin);

public:
    CGTA_Vehicle() = delete;
    CGTA_Vehicle(E_GroupType _eGroupType);
    virtual ~CGTA_Vehicle() override;
};