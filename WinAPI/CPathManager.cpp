#include "stdafx.h"
#include "CPathManager.h"
CPathManager::CPathManager() {
}
CPathManager::~CPathManager() {
}

void CPathManager::Init() {
	wchar_t path[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, path);

	int iLen = wcslen(path);

	for (int i = iLen - 1; i > 0; --i) {
		if (L'\\' == path[i]) {
			path[i] = L'\0';
			break;
		}
	}

	m_strContentPath = path;
	m_strContentPath += STR_FILE_PATH_Content;
}