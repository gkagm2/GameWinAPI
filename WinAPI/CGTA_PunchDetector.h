#pragma once
#include "CObject.h"
class CGTA_PunchDetector : public CObject
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

	virtual void OnCollisionEnter(CObject* _pOther) {}

public:
	CGTA_PunchDetector() = delete;
	CGTA_PunchDetector(E_GroupType e_GroupType);
	virtual ~CGTA_PunchDetector() override;
};