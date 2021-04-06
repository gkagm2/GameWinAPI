#pragma once
#include "CUI.h"
class CPanelUI : public CUI
{
private:

	float m_fTopPadding;
	float m_fBottomPadding;
	float m_fLeftPadding;
	float m_fRightPadding;
	Vector2 m_vSpacing;
	int m_iColCnt;
	int m_iRowCnt;

	Vector3 m_vOriginalPos;
	Vector3 m_vDiff; // Â÷ÀÌ

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	virtual void OnPointerDown() override;
	virtual void OnPointerUp() override;
	//virtual void OnPointerClick() override;

protected:
	void SetGrid(int _iColCnt, int _iRowCnt) { m_iColCnt = _iColCnt; m_iRowCnt = _iRowCnt; }
	void SetPadding(float _fLeft, float _fTop, float _fRight, float _fBottom);
	void SetSpacing(float _fX, float _fY) { m_vSpacing.x = _fX; m_vSpacing.y = _fY; }

public:
	void SetGridUI(int _iColCnt, int _iRowCnt, float _fLeftPadding, float _fTopPadding, float _fRightPadding, float _fBottomPadding, float _fSpacingX, float _fSpacingY);

public:
	CLONE(CPanelUI);

public:
	CPanelUI() = delete;
	CPanelUI(E_GroupType _eGroupType);
	virtual ~CPanelUI() override;
};