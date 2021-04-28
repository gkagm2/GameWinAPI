#include "stdafx.h"
#include "CImageUI.h"
#include "CTexture.h"

CImageUI::CImageUI(E_GroupType _eGroupType) :
	CUI(_eGroupType),
	m_vLT{},
	m_vRB{},
	m_iExceptionColor{EXCEPTION_COLOR_RGB_BLACK},
	m_eRenderType{E_RenderType::transparentBlt}
{
}

CImageUI::~CImageUI()
{
}

void CImageUI::Init()
{
	CUI::Init();
}

void CImageUI::Update()
{
	CUI::Update();
}

void CImageUI::LateUpdate()
{
	CUI::LateUpdate();
}

void CImageUI::Render(HDC _hDC)
{
	if (nullptr == GetTexture())
		return;

	int iWidth = m_vRB.x - m_vLT.x;
	int iHeight = m_vRB.y - m_vLT.y;
	if (E_RenderType::bitBlt == m_eRenderType) {
		BitBlt(_hDC, GetFinalPosition().x, GetFinalPosition().y, iWidth, iHeight, GetTexture()->GetDC(), m_vLT.x, m_vLT.y, SRCCOPY);
	}
	else if (E_RenderType::transparentBlt == m_eRenderType) {
		TransparentBlt(_hDC, GetFinalPosition().x, GetFinalPosition().y, iWidth, iHeight, GetTexture()->GetDC(), m_vLT.x, m_vLT.y, m_vRB.x, m_vRB.y, m_iExceptionColor);
	}
}

void CImageUI::SetImageTransPaBlt(const Vector2& _vLT, const Vector2& _vRT, COLORREF _exceptionColor)
{
	m_eRenderType = E_RenderType::transparentBlt;
	m_vLT = _vLT;
	m_vRB = _vRT;
	m_iExceptionColor = _exceptionColor;
}

void CImageUI::SetImageBitBlt(const Vector2& _vLT, const Vector2& _vRT)
{
	m_eRenderType = E_RenderType::bitBlt;
	m_vLT = _vLT;
	m_vRB = _vRT;
}
