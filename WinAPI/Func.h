#pragma once
// 전역함수

class CObject;
void CreateObject(CObject* _pObject);
void DestroyObject(CObject* _pObject);

void SaveWString(const wstring& _str, FILE* _pFile);
void LoadWString(wstring& _str, FILE* _pFile);

void MenuContainer(WPARAM _wmId);
