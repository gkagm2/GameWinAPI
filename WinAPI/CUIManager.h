#pragma once
class CUI;
class CUIManager
{
	SINGLETON(CUIManager);

	CUI* m_pCurFocusedUI; // 何葛 UI吝俊 器目教等 UI

public:
	void Update();

public:
	void SetCurFocusedUI(CUI* _pCurFocusedUI) { m_pCurFocusedUI = _pCurFocusedUI; }

	CUI* GetFocusedUI(); // 器目教等 UI甫 备茄促.
	CUI* GetTargetedUi(CUI* _pParentUI);


public:
	void UIEvent(CUI* _pParentUI);
};