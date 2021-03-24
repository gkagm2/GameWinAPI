#pragma once


class CDebug
{
	SINGLETON(CDebug);
private:
	wstring m_str;
	list<wstring> m_listLog;
	list<std::pair<Vector2, wstring>> m_listPosLog;

	bool m_bIsShow;
protected:
	void Update();
	void Render(HDC _hDC);
	
public:
	void Print(const wchar_t* types, ...);
	void Print(Vector2 _worldPos, const wchar_t* types, ...);

protected:
	friend class CCore;
};