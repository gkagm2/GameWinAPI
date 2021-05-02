#pragma once
#include "CUI.h"
class CTextUI : public CUI
{
protected:
	enum class E_RenderMode {
		alphaBlend,
		transparentBlt
	};

	enum class E_TextUIPivot {
		leftTop,
		center,
	};

private:
	
	wstring m_strText;
	E_RenderMode m_eRenderMode;
	E_TextUIPivot m_eTextUIPivotState;
	BYTE m_iAlpha; // alphaBlen 모드일 때 사용

public:
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	void SetText(const wstring& _strText) { m_strText = _strText; }
	void SetText(int _iNum);
	void SetText(float _fNum);
	const wstring& GetText() { return m_strText; }

	void SetRenderMode(E_RenderMode _eRenderMode) { m_eRenderMode = _eRenderMode; }
	E_RenderMode GetRenderMode() { return m_eRenderMode; }

	// 알파 블랜드 모드일 때 사용
	void SetAlpha(BYTE _iAlpha) { m_iAlpha = _iAlpha; }
	BYTE GetAlpha() { return m_iAlpha; }

	void SetTextUIPivot(E_TextUIPivot _eTextUIPivot) { m_eTextUIPivotState = _eTextUIPivot; }
	E_TextUIPivot GetTextUIPivotState() { return m_eTextUIPivotState; }

public:
	CLONE(CTextUI);

public:
	CTextUI() = delete;
	CTextUI(E_GroupType _eGroupType);
	virtual ~CTextUI() override;
};