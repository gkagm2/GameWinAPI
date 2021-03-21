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
public:
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	vector<CUI*>& GetChildsUI() { return m_vecChildUI; }
	CUI* GetParentUI() { return m_pParentUI; }

	void SetPivotState(E_UIPivot _ePivot) { m_ePivotState = _ePivot; }
	E_UIPivot GetPivotState(E_UIPivot _ePivot) { return m_ePivotState; }
	void SetPivotPosition(const Vector3& _vPivotPos) { m_vPivotPosition = _vPivotPos; }
	Vector3 GetPivotPosition() { return m_vPivotPosition; }

	// TODO : Min, Max 구하기 구현
	//virtual Vector3 GetMin() override;
	//virtual Vector3 GetMax() override;
public:
	CUI() = delete;
	CUI(E_GroupType _eGroupType);
	virtual ~CUI() override;
};