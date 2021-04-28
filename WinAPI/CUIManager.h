#pragma once
class CUI;
class CUIManager
{
	SINGLETON(CUIManager);

	CUI* m_pCurFocusedUI; // 포커싱되어있는 최상위 UI
	CUI* m_pPointDownUI;  // Point Down된 UI

public:
	void Update();
	void Init() { m_pCurFocusedUI = nullptr, m_pPointDownUI = nullptr; }

public:
	void SetCurFocusedUI(CUI* _pCurFocusedUI) { m_pCurFocusedUI = _pCurFocusedUI; }
	CUI* GetCurFocusedUI() { return m_pCurFocusedUI; }

	bool IsMousePointInUI();
};