#pragma once
#include "CMonster.h"
class CMonster_Small : public CMonster
{
private:

public:
	virtual void Update() override;
	virtual void OnCollisionEnter(CObject* _pOther) override;

	void SetOriginalScale(const Vector3& _vScale) { m_vOriginalScale = _vScale;	}

public:
	CLONE(CMonster_Small);

public:
	CMonster_Small() = delete;
	CMonster_Small(E_GroupType _eGroupType);
	virtual ~CMonster_Small() override;
};