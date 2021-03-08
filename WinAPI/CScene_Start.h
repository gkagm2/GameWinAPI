#pragma once
#include "CScene.h"
class CScene_Start : public CScene
{
public:
	virtual void Start() override;
	virtual void End() override;

public:
	CScene_Start();
	virtual ~CScene_Start() override;
};