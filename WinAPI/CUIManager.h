#pragma once
class CUI;
class CUIManager
{
	SINGLETON(CUIManager);

	CUI* m_pCurFocusedUI; // ��Ŀ�̵Ǿ��ִ� �ֻ��� UI
	CUI* m_pPointDownUI;  // Point Down�� UI

public:
	void Update();
	void Init() { m_pCurFocusedUI = nullptr, m_pPointDownUI = nullptr; }

public:
	void SetCurFocusedUI(CUI* _pCurFocusedUI) { m_pCurFocusedUI = _pCurFocusedUI; }
	CUI* GetCurFocusedUI() { return m_pCurFocusedUI; }

	bool IsMousePointInUI();
};