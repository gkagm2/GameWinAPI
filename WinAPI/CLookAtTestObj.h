#pragma once
#include "CObject.h"
class CLookAtTestObj : public CObject
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	CLONE(CLookAtTestObj);
public:
	CLookAtTestObj() = delete;
	CLookAtTestObj(E_GroupType _eGroupType);
	virtual ~CLookAtTestObj();
};