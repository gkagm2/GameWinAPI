#include "stdafx.h"
#include "CCamera_Tool.h"
#include "CCamera.h"
#include "CKeyManager.h"
#include "CDebug.h"

CCamera_Tool::CCamera_Tool(E_GroupType _eGroupType) :
	CCamera(_eGroupType),
	m_vStartPos{},
	m_vOriginPos{}
{
}

CCamera_Tool::~CCamera_Tool()
{
}

void CCamera_Tool::Init()
{
}

void CCamera_Tool::Update()
{
	const Vector3& vMousePos = Vector3{ MousePosition.x, MousePosition.y };

	if (InputKeyPress(E_Key::MBUTTON)) {
		m_vStartPos = vMousePos;
		m_vOriginPos = m_vLook;
	}
		
	if (InputKeyRelease(E_Key::MBUTTON))
		m_vLook = m_vOriginPos + (m_vStartPos - vMousePos);
		
	if (InputKeyHold(E_Key::MBUTTON)) {
		m_vLook = m_vOriginPos + (m_vStartPos - vMousePos);
		Debug->Print(vMousePos, L"dd", m_vLook.x, m_vLook.y);
	}
	
	__super::Update(); 
}

void CCamera_Tool::Render(HDC _hDC)
{
}