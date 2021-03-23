#pragma once


class CDebug
{
	SINGLETON(CDebug);
private:
	wstring m_str;
	list<wstring> m_vecLog;

	bool m_bIsShow;
protected:
	void Update();
	void Render(HDC _hDC);
	
public:
	void Print(const wchar_t* types, ...);

protected:
	friend class CCore;
};