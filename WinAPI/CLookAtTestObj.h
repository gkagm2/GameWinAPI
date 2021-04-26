#pragma once
#include "CObject.h"
class CLookAtTestObj : public CObject
{
private:
	float angle;
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

	void RotateSizingImage(HDC hdc, HBITMAP hBmp, RECT rt,
		double dblAngle,
		int ixRotateAxis, int iyRotateAxis,
		int ixDisplay, int iyDisplay,
		double dblSizeRatio,
		HBITMAP hMaskBmp, int ixMask, int iyMask);
public:
	CLONE(CLookAtTestObj);
public:
	CLookAtTestObj() = delete;
	CLookAtTestObj(E_GroupType _eGroupType);
	virtual ~CLookAtTestObj();
};