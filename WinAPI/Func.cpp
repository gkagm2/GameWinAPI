#include "stdafx.h"
#include "Func.h"
#include "CEventManager.h"
#include "CObject.h"

void CreateObject(CObject* _pObject)
{
	tEvent evt = {};
	evt.eType = E_EventType::CREATE_OBJECT;
	evt.lParam = (DWORD_PTR)_pObject;
	CEventManager::GetInstance()->AddEvent(evt);
}

void DestroyObject(CObject* _pObject)
{
	tEvent evt = {};
	evt.eType = E_EventType::DELETE_OBJECT;
	evt.lParam = (DWORD_PTR)_pObject;
	CEventManager::GetInstance()->AddEvent(evt);
}


void SaveWString(const wstring& _str, FILE* _pFile) {
	BYTE b = (BYTE)_str.length();
	fwrite(&b, sizeof(BYTE), 1, _pFile);
	fwrite(_str.c_str(), sizeof(wchar_t), (size_t)b, _pFile);
}
void LoadWString(wstring& _str, FILE* _pFile) {
	wchar_t szBuffer[255] = {};
	BYTE b = 0;
	fread(&b, sizeof(BYTE), 1, _pFile);
	fread(szBuffer, sizeof(wchar_t), (size_t)b, _pFile);
	_str = szBuffer;
}