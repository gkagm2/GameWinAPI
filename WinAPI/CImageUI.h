#pragma once
#include "CUI.h"

enum class E_RenderType {
	transparentBlt,
	bitBlt,
	// plgBlt,
};
class CImageUI : public CUI
{
private:
	Vector2 m_vLT; // image's left top position
	Vector2 m_vRB; // image's right bottom position

	COLORREF m_iExceptionColor;
	E_RenderType m_eRenderType;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	//virtual void OnPointerDown() override;
	//virtual void OnPointerUp() override;
	//virtual void OnPointerClick() override;

public:
	void SetImageTransPaBlt(const Vector2& _vTexLT, const Vector2& _vTexRT, const Vector3& _vScale, COLORREF _exceptionColor = EXCEPTION_COLOR_RGB_BLACK); // TransparentBlt 용도
	void SetImageBitBlt(const Vector2& _vTexLT, const Vector2& _vTexRT); //BitBlt 용도

	void SetLT(Vector2 _vLeftTopPos) { m_vLT = _vLeftTopPos; }
	Vector2 GetLT() { return m_vLT; }
	void SetRB(Vector2 _vRightBottomPos) { m_vRB = _vRightBottomPos; }
	Vector2 GetRB() { return m_vRB; }

	void SetExceptionColor(COLORREF _exceptionColor) { m_iExceptionColor = _exceptionColor; }

	E_RenderType GetCurRenderType() { return m_eRenderType; }

public:
	CLONE(CImageUI);
public:
	CImageUI() = delete;
	CImageUI(E_GroupType _eGroupType);
	virtual ~CImageUI() override;
};