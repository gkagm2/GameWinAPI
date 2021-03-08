#include "stdafx.h"
#include "CResourceManager.h"
#include "CPathManager.h"

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

CSound* CResourceManager::LoadSound(const wstring& _strKey, const wstring& _strRelativePath)
{
	umapSoundIter iter = m_umapSound.find(_strKey);
	// TODO (Sagacity Jang)
	return nullptr;
}
