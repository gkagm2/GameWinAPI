#include "stdafx.h"
#include "CDebug.h"
#include "CKeyManager.h"

CDebug::CDebug() :
	m_str(L""),
	m_bIsShow(false)
{
}
CDebug::~CDebug() {

}

void CDebug::Update()
{
	if (InputKeyHold(E_Key::LSHIFT) && InputKeyPress(E_Key::F5))
		m_bIsShow = !m_bIsShow;
}

void CDebug::Render(HDC _hDC)
{
	if (false == m_bIsShow)
		return;

	int originalMode = GetBkMode(_hDC); // TRANSPARENT
	COLORREF originalColor = GetTextColor(_hDC);
	SetBkMode(_hDC, TRANSPARENT);
	SetTextColor(_hDC, RGB(255, 0, 0));
	TextOut(_hDC, 10, 2, L"DEBUG LOG", wcslen(L"DEBUG LOG"));

	int y = 1;
	for (auto iter = m_vecLog.begin(); iter != m_vecLog.end(); ++iter) {
		TextOut(_hDC, 10, 20 * y, iter->c_str(), wcslen(iter->c_str()));
		++y;
	}

	while (!m_vecLog.empty())
		m_vecLog.pop_back();
	SetTextColor(_hDC, RGB(255, 0, 0));
	SetTextColor(_hDC, originalColor);
	SetBkMode(_hDC, originalMode);
}

void CDebug::Print(const wchar_t* types, ...)
{
	va_list ap;
	int i = 0;

	va_start(ap, types);
	wstring strLog;
	
	while (types[i] != '\0') {

		switch (types[i]) {
		case 'i':
		{
			wchar_t buffer[255]{};
			swprintf(buffer, 255, L"%d ", va_arg(ap, int));
			strLog = strLog + buffer;
		}
			break;
		case 'd':
		{
			wchar_t buffer[255]{};
			swprintf(buffer, 255, L"%f ", float(va_arg(ap, double)));
			strLog = strLog + buffer;
		}
			break;
		case 'c':
			strLog += va_arg(ap, wchar_t);
			break;
		case 's':
			strLog += va_arg(ap, wchar_t*);
			break;
		}
		++i;
	}
	
	m_vecLog.push_back(strLog);

	va_end(ap);
}