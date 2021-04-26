#include "stdafx.h"
#include "CResourceManager.h"
#include "CPathManager.h"

#include "CResource.h"

#include "CTexture.h"
#include "CSound.h"

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager() {
	umapTextureIter textureIter = m_umapTexture.begin();
	for (; textureIter!= m_umapTexture.end(); ++textureIter)
		delete textureIter->second;

	umapSoundIter soundIter = m_umapSound.begin();
	for (; soundIter != m_umapSound.end(); ++soundIter)
		delete soundIter->second;
}

CTexture* CResourceManager::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{
	CResource* pTexture = FindTexture(_strKey);
	if (nullptr != pTexture) {
		wstring errorContent = STR_TABLE_TextureOverlapLoadingProblem + _strRelativePath;
		MessageBox(nullptr, errorContent.c_str(), STR_TABLE_FailTextureLoading, MB_OK);
		return (CTexture*)(pTexture);
	}

	wstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativePath;
	pTexture = new CTexture;

	HRESULT hResult = pTexture->Load(strFilePath.c_str());
	if (FAILED(hResult)) {
		wstring errorContent = STR_TABLE_FailTextureLoading + _strRelativePath;
		MessageBox(nullptr, errorContent.c_str(), STR_TABLE_FailResourceLoading, MB_OK);
		delete pTexture;
		return nullptr;
	}

	pTexture->SetKey(_strKey);
	pTexture->SetRelativePath(_strRelativePath);
	m_umapTexture.insert(make_pair(_strKey, (CTexture*)pTexture));

	return (CTexture*)pTexture;
}

CTexture* CResourceManager::FindTexture(const wstring& _strKey)
{
	umapTextureIter iter = m_umapTexture.find(_strKey);
	if (iter == m_umapTexture.end())
		return nullptr;

	return iter->second;
}

CTexture* CResourceManager::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight)
{
	CResource* pTexture = FindTexture(_strKey);
	if (nullptr != pTexture) {
		wstring errorContent = STR_TABLE_TextureOverlapLoadingProblem;
		MessageBox(nullptr, errorContent.c_str(), STR_TABLE_FailTextureLoading, MB_OK);
		return (CTexture*)(pTexture);
	}

	pTexture = new CTexture;
	((CTexture*)pTexture)->Create(_iWidth, _iHeight);
	pTexture->SetKey(_strKey);
	m_umapTexture.insert(make_pair(_strKey, (CTexture*)pTexture));
	return (CTexture*)pTexture;
}

CSound* CResourceManager::LoadSound(const wstring& _strKey, const wstring& _strRelativePath)
{
	assert(nullptr == FindSound(_strKey));

	CSound* pSound = new CSound;

	wstring strFilePath = CPathManager::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;
	HRESULT hr = pSound->Load(strFilePath.c_str());

	if (FAILED(hr))
	{
		MessageBox(nullptr, STR_TABLE_FailSoundLoading, STR_TABLE_FailResourceLoading, MB_OK);
		delete pSound;
		return nullptr;
	}

	pSound->SetKey(_strKey);
	pSound->SetRelativePath(_strRelativePath);

	m_umapSound.insert(make_pair(_strKey, pSound)); // map ¿¡ µî·Ï

	return pSound;
}

CSound* CResourceManager::FindSound(const wstring& _strKey)
{
	unordered_map<wstring, CSound*>::iterator iter = m_umapSound.find(_strKey);

	if (iter == m_umapSound.end())
		return nullptr;

	return iter->second;
}
