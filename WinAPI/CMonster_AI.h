#pragma once
#include "CMonster.h"
class CMonster_AI : public CMonster
{
private :

public:
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	CLONE(CMonster_AI);

public:
	CMonster_AI() = delete;
	CMonster_AI(E_GroupType _eGroupType);
	virtual ~CMonster_AI() override;
};