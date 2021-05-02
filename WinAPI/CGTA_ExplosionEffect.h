#pragma once
#include "CObject.h"
class CGTA_ExplosionEffect : public CObject
{

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC);

public:
	CLONE(CGTA_ExplosionEffect);

public:
	CGTA_ExplosionEffect() = delete;
	CGTA_ExplosionEffect(E_GroupType _eGroupType);
	virtual ~CGTA_ExplosionEffect() override;
};