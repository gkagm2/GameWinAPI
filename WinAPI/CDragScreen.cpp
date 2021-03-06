#include "stdafx.h"
#include "CDragScreen.h"
#include "CKeyManager.h"
#include "CCamera.h"
#include "CScene.h"
#include "CSceneManager.h"

CDragScreen::CDragScreen(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_bIsPressed(false),
	m_vStartPos{},
	m_bIsEnableDrag(true)
{
}

CDragScreen::~CDragScreen()
{
}

void CDragScreen::Update()
{
	if (m_bIsEnableDrag) {
		if (InputKeyPress(E_Key::LBUTTON)) {
			m_bIsPressed = true;

			m_vStartPos = MainCamera->GetScreenToWorldPosition(MousePosition);
		}
		if (InputKeyRelease(E_Key::LBUTTON)) {
			m_bIsPressed = false;
		}
	}
}

void CDragScreen::Render(HDC _hDC)
{
	if (true == m_bIsPressed) { // ????????
		HPEN hPen = CreatePen(PS_DOT, 1, RGB(128, 128, 1280));
		HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);
		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);

		Vector2 vStartRenderPos = MainCamera->GetRenderPosition(m_vStartPos);
		Rectangle(_hDC, (int)vStartRenderPos.x, (int)vStartRenderPos.y, (int)MousePosition.x, (int)MousePosition.y);
		
		SelectObject(_hDC, oldBrush);
		SelectObject(_hDC, hOldPen);
		DeleteObject(myBrush);
		DeleteObject(hPen);
	}
}

void CDragScreen::OnMousePressed()
{
	m_bIsPressed = true;
	m_vStartPos = MousePosition;
}

void CDragScreen::SetDragScreenEnable(bool _bEnable)
{
	m_bIsEnableDrag = _bEnable;
}
