#include "stdafx.h"
#include "CTextUI.h"
#include "CResource.h"
#include "CResourceManager.h"
#include "CTexture.h"

CTextUI::CTextUI(E_GroupType _eGroupType) :
	CUI(_eGroupType),
	m_strText(L"")
{
	CTexture* pFontTex = CResourceManager::GetInstance()->FindTexture(STR_FILE_PATH_GTA_Font);
	if (nullptr == pFontTex) {
		pFontTex = CResourceManager::GetInstance()->LoadTexture(STR_FILE_PATH_GTA_Font, STR_FILE_PATH_GTA_Font);
		assert(pFontTex);
	}
	SetTexture(pFontTex);
}

CTextUI::~CTextUI()
{
}

void CTextUI::Update()
{
}

void CTextUI::Render(HDC _hDC)
{
	if (nullptr == GetTexture())
		return;

	Vector3 vCursorPos = GetPosition();
	for (UINT i = 0; i < m_strText.size(); ++i) {
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
		int a = c;
		// 맵핑을 하게되면?
		TransparentBlt(_hDC, vCursorPos.x, vCursorPos.y, GetScale().x, GetScale().y, GetTexture()->GetDC(), c * FONT_SIZE, 0, FONT_SIZE, FONT_SIZE, EXCEPTION_COLOR_RGB_BLACK);
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