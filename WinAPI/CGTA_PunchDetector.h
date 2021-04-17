#pragma once
#include "CObject.h"
class CGTA_PunchDetector : public CObject
{

public:
	virtual void Init() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(CObject* _pOther)override;
	virtual void OnCollisionStay(CObject* _pOther) override;
	virtual void OnCollisionExit(CObject* _pOther) override;

public:
	CLONE(CGTA_PunchDetector);

public:
	CGTA_PunchDetector() = delete;
	CGTA_PunchDetector(E_GroupType _eGroupType);
	virtual ~CGTA_PunchDetector() override;
};