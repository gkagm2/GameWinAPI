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

// Tile tool sceneC
#include "CSceneManager.h"
#include "CScene.h"
#include "CScene_Tool.h"
INT_PTR CALLBACK CreateTileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CharacterTool(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
HWND hWndTool = nullptr;
void MenuContainer(WPARAM _wmId)
{
	HWND hWnd = CCore::GetInstance()->GetWndHandle();

	switch (_wmId) {
	case ID_FILE_SAVE:
		// Tile Info
		// Player Info
		// Objects Info
		// Vehicle
		// Cive

		break;
	case ID_FILE_LOAD:
	{
		CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(CSceneManager::GetInstance()->GetCurScene());
		assert(pToolScene);
		pToolScene->LoadTile(STR_FILE_PATH_Tile_Save);
		pToolScene->LoadAll(STR_FILE_PATH_MetaData);
	}
		break;

	case ID_TILE_CREATE:
		DialogBox(nullptr, MAKEINTRESOURCE(IDD_CREATE_TILE_DIALOG), hWnd, CreateTileProc);
		break;
	case ID_TILE_LOAD:
	{
		OPENFILENAME ofn;
		wchar_t strMaxPath[MAX_PATH] = L"";
		memset(&ofn, 0, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = CCore::GetInstance()->GetWndHandle();
		ofn.lpstrFilter = L"葛电颇老(*.*)\0*.*\0*";
		ofn.lpstrFile = strMaxPath;
		ofn.nMaxFile = MAX_PATH;

		
		if (0 != GetOpenFileName(&ofn)) {
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(CSceneManager::GetInstance()->GetCurScene());
			assert(pToolScene);
			pToolScene->LoadTile(ofn.lpstrFile);

			wchar_t str[255] = L"File Load";
			MessageBox(CCore::GetInstance()->GetWndHandle(), str, L"Load", MB_OK);
		}
	}
		break;
	case ID_TILE_SAVE:
	{
		OPENFILENAME ofn;
		wchar_t strMaxPath[MAX_PATH] = L"";
		memset(&ofn, 0, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = CCore::GetInstance()->GetWndHandle();
		ofn.lpstrFilter = L"葛电颇老(*.*)\0*.*\0*";
		ofn.lpstrFile = strMaxPath;
		ofn.nMaxFile = MAX_PATH;

		
		if (0 != GetSaveFileName(&ofn)) {
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(CSceneManager::GetInstance()->GetCurScene());
			assert(pToolScene);
			pToolScene->SaveTile(ofn.lpstrFile);

			wchar_t str[255] = L"File Save";
			MessageBox(CCore::GetInstance()->GetWndHandle(), str, L"Save", MB_OK);
		}
	}
		break;
	case ID_CHARACTER_TOOL:
		// Modeless dialog
		if (!IsWindow(hWndTool)) {
			hWndTool = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_CHACTER_TOOL), hWnd, CharacterTool);
			ShowWindow(hWndTool, SW_SHOW);
		}
		else
			SetFocus(hWndTool);
		InvalidateRect(hWndTool, nullptr, true);
		break;
		
	default:
		break;
	}
}