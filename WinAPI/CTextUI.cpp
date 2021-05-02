#include "stdafx.h"
#include "CTextUI.h"
#include "CResource.h"
#include "CResourceManager.h"
#include "CTexture.h"

CTextUI::CTextUI(E_GroupType _eGroupType) :
	CUI(_eGroupType),
	m_strText(L""),
	m_eRenderMode(E_RenderMode::transparentBlt),
	m_eTextUIPivotState(E_TextUIPivot::leftTop), // 기본값 왼쪽 위
	m_iAlpha(255) // 기본값 255 : 불투명
	
{
	CTexture* pFontTex = CResourceManager::GetInstance()->GetTexture(STR_FILE_PATH_GTA_Font, STR_FILE_PATH_GTA_Font);
	SetTexture(pFontTex);
}

CTextUI::~CTextUI()
{
}

void CTextUI::Update()
{
	CUI::Update();
}

void CTextUI::Render(HDC _hDC)
{
	if (nullptr == GetTexture())
		return;

	Vector3 vCursorPos = GetFinalPosition();
	int iTextSize = m_strText.size();
	for (UINT i = 0; i < iTextSize; ++i) {
		wchar_t c = m_strText[i];
		if (L'a' <= m_strText[i] && L'z' >= m_strText[i]) {
			c = 57 - (L'z' - m_strText[i]);
		}
		else if (91 <= m_strText[i] && 96 >= m_strText[i]) {
			c = 58 + ( m_strText[i] - 91);
		}
		else if (123 <= m_strText[i] && 126 >= m_strText[i]) {
			c = 64 + (m_strText[i] - 123);
		}
		else if (L'\n' == c) {
			vCursorPos.y += ScaleY();
			vCursorPos.x = PosX();
			continue;
		}
		else
			c = c - L'!';

		float offsetX = 0.f;
		float offsetY = 0.f;
		if (E_TextUIPivot::center == m_eTextUIPivotState) {
			offsetX = -(iTextSize * GetScale().x * 0.5f);
			offsetY = -(GetScale().y * 0.5f);
		}

		if (E_RenderMode::alphaBlend == m_eRenderMode) {
			// 알파를 추가해서 전체적으로 사라지게 하기
			BLENDFUNCTION tBlendFunc;
			tBlendFunc.BlendOp = AC_SRC_OVER;
			tBlendFunc.BlendFlags = 0;				// 반드시 0
			tBlendFunc.SourceConstantAlpha = m_iAlpha;	// 0 : 투명, 255 불투명
			tBlendFunc.AlphaFormat = AC_SRC_ALPHA;	// AC_SRC_ALPHA

			AlphaBlend(_hDC,
				(int)vCursorPos.x + offsetX, (int)vCursorPos.y + offsetY, (int)GetScale().x, (int)GetScale().y,
				GetTexture()->GetDC(),
				c * FONT_SIZE, 0, (int)FONT_SIZE, (int)FONT_SIZE,
				tBlendFunc);
		}
		else if (E_RenderMode::transparentBlt == m_eRenderMode) {
			TransparentBlt(_hDC, (int)vCursorPos.x + offsetX, (int)vCursorPos.y + offsetY, (int)GetScale().x, (int)GetScale().y, GetTexture()->GetDC(), c * FONT_SIZE, 0, FONT_SIZE, FONT_SIZE, EXCEPTION_COLOR_RGB_BLACK);
		}
		
		vCursorPos.x += GetScale().x -10;
	}
}

void CTextUI::SetText(int _iNum)
{
	wchar_t buff[255];
	swprintf(buff, 255, L"%d", _iNum);
	m_strText = buff;
}

void CTextUI::SetText(float _fNum)
{
	wchar_t buff[255];
	swprintf(buff, 255, L"%.2f", _fNum);
	m_strText = buff;
}