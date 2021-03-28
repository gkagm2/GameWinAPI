#pragma once
#include "CObject.h"
#include "CTexture.h"
class CBackground : public CObject
{
public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	void InitTexture();

public:
	CLONE(CBackground);
public:
	CBackground() = delete;
	CBackground(E_GroupType e_GroupType);
	virtual ~CBackground() override;
};