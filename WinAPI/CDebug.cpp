#include "stdafx.h"
#include "CDebug.h"
#include "CKeyManager.h"
#include "CCamera.h"

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
	TextOut(_hDC, 10, 2, L"DEBUG LOG", (int)wcslen(L"DEBUG LOG"));

	// static log
	int y = 1;
	for (auto iter = m_listLog.begin(); iter != m_listLog.end(); ++iter) {
		TextOut(_hDC, 10, 20 * y, iter->c_str(), (int)wcslen(iter->c_str()));
		++y;
	}

	// position log
	for (auto iter = m_listPosLog.begin(); iter != m_listPosLog.end(); ++iter) {
		TextOut(_hDC, (int)iter->first.x, (int)iter->first.y, iter->second.c_str(), (int)wcslen(iter->second.c_str()));
	}

	while (!m_listLog.empty())
		m_listLog.pop_back();
	while (!m_listPosLog.empty())
		m_listPosLog.pop_back();

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
	
	m_listLog.push_back(strLog);

	va_end(ap);
}

void CDebug::Print(Vector2 _worldPos, const wchar_t* types, ...)
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

	m_listPosLog.push_back(std::make_pair(_worldPos, strLog));

	va_end(ap);
}