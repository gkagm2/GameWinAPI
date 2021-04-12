#pragma once
#include "CGTA_Vehicle.h"
class CGTA_PoliceCar : public CGTA_Vehicle
{
public:
	virtual void Init() override;
	virtual void PrevUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

	virtual void OnCollisionEnter(CObject* _pOther) override;
	virtual void OnCollisionStay(CObject* _pOther) override;
	virtual void OnCollisionExit(CObject* _pOther) override;

public:
	CLONE(CGTA_PoliceCar);
public:
	CGTA_PoliceCar() = delete;
	CGTA_PoliceCar(E_GroupType e_GroupType);
	virtual ~CGTA_PoliceCar() override;
};