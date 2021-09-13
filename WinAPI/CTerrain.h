#pragma once
#include "CObject.h"
class CTerrain : public CObject
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;
public:
	CLONE(CTerrain);
	CTerrain() = delete;
	CTerrain(E_GroupType _eGroupType);
	virtual ~CTerrain() override;
};