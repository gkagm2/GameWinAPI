#pragma once
#include "CObject.h"
class CGTA_Vehicle;
class CGTA_Character : public CObject
{
private:
	CGTA_Vehicle* m_pVehicle;

public:
	virtual void Init() override;
	virtual void PrevUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

	virtual void OnCollisionEnter(CObject* _pOther);
	virtual void OnCollisionStay(CObject* _pOther);
	virtual void OnCollisionExit(CObject* _pOther);

public:
	virtual void Move() {};
	virtual void Shoot() {};
	virtual void Drive() {};
	virtual void Punch() {};
	virtual void Dead() {};
	virtual void GetInTheVehicle() {};
	virtual void GetOutTheVehicle() {};


public:
	CLONE(CGTA_Character);

public:
	CGTA_Character() = delete;
	CGTA_Character(E_GroupType _eGroupType);
	virtual ~CGTA_Character() override;
};

