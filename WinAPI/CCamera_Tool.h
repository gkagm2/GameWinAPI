#pragma once
#include "CCamera.h"
class CCamera_Tool : public CCamera
{
private:
	Vector3 m_vStartPos;
	Vector3 m_vOriginPos;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	//CLONE(CCamera_Tool);

public:
	CCamera_Tool() = delete;
	CCamera_Tool(E_GroupType _eGroupType);
	virtual ~CCamera_Tool() override;
};