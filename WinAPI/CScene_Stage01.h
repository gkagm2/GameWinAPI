#pragma once
#include "CScene.h"
class CScene_Stage01 : public CScene
{
public:
	virtual void Start() override;
	virtual void End() override;

public:
	CScene_Stage01();
	virtual ~CScene_Stage01();
};