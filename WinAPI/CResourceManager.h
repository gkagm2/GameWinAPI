#pragma once

class CTexture;
class CSound;
class CResourceManager
{
	SINGLETON(CResourceManager)
private:
	typedef unordered_map<wstring, CTexture*>::iterator umapTextureIter;
	typedef unordered_map<wstring, CSound*>::iterator umapSoundIter;

	unordered_map<wstring, CTexture*> m_umapTexture;
	unordered_map<wstring, CSound*> m_umapSound;

public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
	CTexture* FindTexture(const wstring& _strKey);
	CTexture* CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight);
	CTexture* GetTexture(const wstring& _strKey, const wstring& _strRelativePath);

	CSound* LoadSound(const wstring& _strKey, const wstring& _strRelativePath);
	CSound* FindSound(const wstring& _strKey);
	CSound* GetSound(const wstring& _strKey, const wstring& _strRelativePath);

public:
	void DeleteAllResources();
	void InitAllSounds();
};