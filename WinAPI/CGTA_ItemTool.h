#pragma once
#include "CObject.h"
class CGTA_ItemTool : public CObject
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

	//virtual void OnCollisionEnter(CObject* _pOther) override;
	//virtual void OnCollisionStay(CObject* _pOther) override;
	//virtual void OnCollisionExit(CObject* _pOther) override;

public:
	CGTA_ItemTool() = delete;
	CGTA_ItemTool(E_GroupType _eGroupType);
	virtual ~CGTA_ItemTool();
};