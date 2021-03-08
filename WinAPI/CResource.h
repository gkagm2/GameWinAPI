#pragma once
class CResource
{
private:
	wstring m_strKey;		   // ���� Ű
	wstring m_strRelativePath; // ��� ���

private:
	virtual int Load(const wchar_t* _pFilePath) = 0;
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void SetKey(const wchar_t* _pStrKey) { m_strKey = _pStrKey; }
	void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }
	void SetRelativePath(const wchar_t* _pStrRelativePath) { m_strRelativePath = _pStrRelativePath; }


public:
	CResource();
	~CResource();

	friend class CResourceManager;
};

