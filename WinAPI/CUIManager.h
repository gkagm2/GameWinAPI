#pragma once
class CUI;
class CUIManager
{
	SINGLETON(CUIManager);

	CUI* m_pCurFocusedUI; // ��Ŀ�̵Ǿ��ִ� �ֻ��� UI
	CUI* m_pPointDownUI;  // Point Down�� UI

public:
	void Update();

public:
	void SetCurFocusedUI(CUI* _pCurFocusedUI) { m_pCurFocusedUI = _pCurFocusedUI; }
};