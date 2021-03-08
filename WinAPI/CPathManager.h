#pragma once
class CPathManager
{
	SINGLETON(CPathManager)
private:
	wstring m_strContentPath;
public:
	void Init();
	const wchar_t* GetContentPath() {
		return m_strContentPath.c_str();
	 }
};