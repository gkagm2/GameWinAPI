#pragma once
class CUI;
class CUIManager
{
	SINGLETON(CUIManager);

	CUI* m_pCurFocusedUI; // �θ� UI�߿� ��Ŀ�̵� UI

public:
	void Update();

public:
	void SetCurFocusedUI(CUI* _pCurFocusedUI) { m_pCurFocusedUI = _pCurFocusedUI; }

	CUI* GetFocusedUI(); // ��Ŀ�̵� UI�� ���Ѵ�.
	CUI* GetTargetedUi(CUI* _pParentUI);


public:
	void UIEvent(CUI* _pParentUI);
};