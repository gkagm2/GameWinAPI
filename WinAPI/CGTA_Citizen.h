#pragma once
#include "CGTA_Character.h"
class CGTA_Citizen : public CGTA_Character
{
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
	virtual void Move() override;
	virtual void Shoot() override;
	virtual void Drive() override;
	virtual void Punch() override;
	virtual void Dead() override;
	virtual void GetInTheVehicle() override;
	virtual void GetOutTheVehicle() override;

public:
	CLONE(CGTA_Citizen);

public:
	CGTA_Citizen() = delete;
	CGTA_Citizen(E_GroupType _eGroupType);
	virtual ~CGTA_Citizen() override;
};