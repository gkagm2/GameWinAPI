#pragma once
#include "CObject.h"
class CDragScreen : public CObject
{
private:
	bool m_bIsPressed;
	Vector2 m_vStartPos;
	bool m_bIsEnableDrag;
	
public:
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	void OnMousePressed();
	void OnMouseReleased() { m_bIsPressed = false; }

	bool IsActiveDrag() { m_bIsEnableDrag; }
	void SetDragScreenEnable(bool _bEnable);


public:
	CLONE_DISABLE(CDragScreen);
public:
	CDragScreen() = delete;
	CDragScreen(E_GroupType _eGroupType);
	virtual ~CDragScreen() override;
};