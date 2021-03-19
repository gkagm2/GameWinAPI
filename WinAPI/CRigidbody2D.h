#pragma once
#include "CRigidbody.h"
class CObject;
class CRigidbody2D : public CRigidbody
{
public:
	virtual void PrevUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

public:
	CRigidbody2D() = delete;
	CRigidbody2D(CObject* _pTargetObj);
	virtual ~CRigidbody2D() override;
};