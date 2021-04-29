#pragma once
#include "CUI.h"
class CTextUI : public CUI
{
private:
	wstring m_strText;
public:
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	void SetText(const wstring& _strText) { m_strText = _strText; }
	void SetText(int _iNum);
	void SetText(float _fNum);

public:
	CLONE(CTextUI);

public:
	CTextUI() = delete;
	CTextUI(E_GroupType _eGroupType);
	virtual ~CTextUI() override;
};