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

	int iWidth = (int)(m_vRB.x - m_vLT.x);
	int iHeight = (int)(m_vRB.y - m_vLT.y);
	if (E_RenderType::bitBlt == m_eRenderType) {
		BitBlt(_hDC, (int)GetFinalPosition().x, (int)GetFinalPosition().y, (int)GetScale().x, (int)GetScale().y, GetTexture()->GetDC(), (int)m_vLT.x, (int)m_vLT.y, SRCCOPY);
	}
	else if (E_RenderType::transparentBlt == m_eRenderType) {
		TransparentBlt(_hDC, (int)GetFinalPosition().x, (int)GetFinalPosition().y, (int)GetScale().x, (int)GetScale().y, GetTexture()->GetDC(), (int)m_vLT.x, (int)m_vLT.y, (int)m_vRB.x, (int)m_vRB.y, m_iExceptionColor);
	}
}

// 설명 : TransparentBlt 이용 시 Scale값을 이미지 크기로 정한다.
void CImageUI::SetImageTransPaBlt(const Vector2& _vTexLT, const Vector2& _vTexRT, const Vector3& _vScale, COLORREF _exceptionColor)
{
	
	m_eRenderType = E_RenderType::transparentBlt;
	m_vLT = _vTexLT;
	m_vRB = _vTexRT;
	SetScale(_vScale);
	m_iExceptionColor = _exceptionColor;
}

// 설명 : BitBlt 이용 시 텍스쳐의 width, height을 이미지 크기로 정한다.
void CImageUI::SetImageBitBlt(const Vector2& _vTexLT, const Vector2& _vTexRT)
{
	
	m_eRenderType = E_RenderType::bitBlt;
	m_vLT = _vTexLT;
	m_vRB = _vTexRT;
}
