#include "stdafx.h"
#include "Func.h"
#include "CEventManager.h"
#include "CObject.h"
#include "CCore.h"

#include "resource.h"

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

// Tile tool scene
#include "CSceneManager.h"
#include "CScene.h"
#include "CScene_Tool.h"
INT_PTR CALLBACK CreateTileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void MenuContainer(WPARAM _wmId)
{
	HWND hWnd = CCore::GetInstance()->GetWndHandle();

	switch (_wmId) {
	case ID_TILE_CREATE:
		DialogBox(nullptr, MAKEINTRESOURCE(IDD_CREATE_TILE_DIALOG), hWnd, CreateTileProc);
		break;
	case ID_TILE_LOAD:
	{
		CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(CSceneManager::GetInstance()->GetCurScene());
		assert(pToolScene);
		pToolScene->LoadTile(STR_FILE_PATH_Tile_Save);
	}
		break;
	case ID_TILE_SAVE:
	{
		CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(CSceneManager::GetInstance()->GetCurScene());
			assert(pToolScene);
		pToolScene->SaveTile(STR_FILE_PATH_Tile_Save);
	}
		break;
	default:
		break;
	}
}