#pragma once
#include "CGTA_Character.h"
class CGTA_Cop : public CGTA_Character
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
	virtual void State() override;
	virtual void Drive() override;
	virtual void GetInTheVehicle() override;
	virtual void GetOutTheVehicle() override;

	virtual void HitByFist() override;
	virtual void Dead() override;
	virtual void Wander() override;
	virtual void Trace() override;

public:
	CLONE(CGTA_Cop);

public:
	CGTA_Cop() = delete;
	CGTA_Cop(E_GroupType _eGroupType);
	virtual ~CGTA_Cop() override;
};