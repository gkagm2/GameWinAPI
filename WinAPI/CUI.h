#pragma once
#include "CObject.h"

enum class E_UIPivot {
	leftTop,
	left,
	leftBottom,
	top,
	center,
	botton,
	rightTop,
	right,
	rightBottom
};

class CUI : public CObject
{
private:
	vector<CUI*> m_vecChildUI;
	CUI* m_pParentUI;

	E_UIPivot m_ePivotState;
	Vector3 m_vPivotPosition;

	bool m_bIsOn;

public:
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	// TODO : 이걸 다른걸로 뺄까?
	// EventTrigger
	virtual void OnPointerEnter();
	virtual void OnPointerExit();
	virtual void OnPointerDown();
	virtual void OnPointerUp();
	virtual void OnPointerClick();
	/*virtual void OnPointerMove();
	virtual void OnPointerBeginDrag();
	virtual void OnPointerDrag();
	virtual void OnPointerEndDrag();*/

	bool IsPointerOn(const Vector2& _vPointerPosition);

public:
	void AddChildUI(CUI* _pChildUI) {
		_pChildUI->m_pParentUI = this;
		m_vecChildUI.push_back(_pChildUI);
	}
	
	vector<CUI*>& GetChildsUI() { return m_vecChildUI; }
	CUI* GetParentUI() { return m_pParentUI; }

	void SetPivotState(E_UIPivot _ePivot) { m_ePivotState = _ePivot; }
	E_UIPivot GetPivotState(E_UIPivot _ePivot) { return m_ePivotState; }
	void SetPivotPosition(const Vector3& _vPivotPos) { m_vPivotPosition = _vPivotPos; }
	Vector3 GetPivotPosition() { return m_vPivotPosition; }

	// TODO : Min, Max 구하기 구현
	virtual Vector3 GetMin() override;
	virtual Vector3 GetMax() override;
public:
	CUI() = delete;
	CUI(E_GroupType _eGroupType);
	virtual ~CUI() override;

public:
	friend class CUIManager;
};