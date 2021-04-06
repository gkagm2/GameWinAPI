#pragma once
#include "CObject.h"
class CGTA_Player : public CObject
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
	CLONE(CGTA_Player);

public:
	CGTA_Player() = delete;
	CGTA_Player(E_GroupType _eGroupType);
	virtual ~CGTA_Player() override;
};