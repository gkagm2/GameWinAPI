#pragma once
class CResource
{
private:
	wstring m_strKey;		   // 고유 키
	wstring m_strRelativePath; // 상대 경로

private:
	virtual int Load(const wchar_t* _pFilePath) = 0;
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void SetKey(const wchar_t* _pStrKey) { m_strKey = _pStrKey; }
	void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }
	void SetRelativePath(const wchar_t* _pStrRelativePath) { m_strRelativePath = _pStrRelativePath; }

public:
	const wstring& GetKey() const { return m_strKey; }
	const wstring& GetRelativePath() const { return m_strRelativePath; }

public:
	CResource();
	virtual ~CResource();

	friend class CResourceManager;
};