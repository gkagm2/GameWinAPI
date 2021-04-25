#include "stdafx.h"
#include "Func.h"
#include "CEventManager.h"
#include "CObject.h"
#include "CCore.h"
#include "CObjTool.h"

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
INT_PTR CALLBACK CharacterTool(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ItemTool(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ObjectTool(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND hWndItemTool = nullptr;
HWND hWndObjectTool = nullptr;
void MenuContainer(WPARAM _wmId)
{
	HWND hWnd = CCore::GetInstance()->GetWndHandle();

	if (hWndItemTool) {
		CloseWindow(hWndItemTool);
		//InvalidateRect(hWndItemTool, nullptr, true);
		hWndItemTool = nullptr;
	}
	if (hWndObjectTool) {
		CloseWindow(hWndObjectTool);
		hWndObjectTool = nullptr;
	}
		
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
		pToolScene->LoadTile(STR_FILE_PATH_GTA_Tile_Save);
		pToolScene->LoadAll();
	}
		break;

	case ID_TILE_CREATE:
		DialogBox(nullptr, MAKEINTRESOURCE(IDD_CREATE_TILE_DIALOG), hWnd, CreateTileProc);
		break;

	// TILE // 
	case ID_TILE_TOOL:
	{
		vector<CObject*>& vecObj = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::PREV);
		CObjTool* pObjTool = nullptr;
		for (int i = 0; vecObj.size(); ++i) {
			pObjTool = dynamic_cast<CObjTool*>(vecObj[i]);
			if (pObjTool)
				break;
		}
		assert(pObjTool);

		if (nullptr == pObjTool)
			break;
		pObjTool->OpenMapTool();
		break;
	}
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
			pToolScene->LoadTileDialogBox(ofn.lpstrFile);

			wchar_t str[255] = L"Tile File Load";
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

			wchar_t str[255] = L"Tile File Save";
			MessageBox(CCore::GetInstance()->GetWndHandle(), str, L"Save", MB_OK);
		}
		break;
	}

	// ITEM //
	case ID_ITEM_TOOL:
	{
		// Modeless dialog
		if (!IsWindow(hWndItemTool)) {
			hWndItemTool = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_ITEM_TOOL), hWnd, ItemTool);
			ShowWindow(hWndItemTool, SW_SHOW);
		}
		else
			SetFocus(hWndItemTool);
		InvalidateRect(hWndItemTool, nullptr, true);

		vector<CObject*>& vecObj = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::PREV);
		CObjTool* pObjTool = nullptr;
		for (int i = 0; vecObj.size(); ++i) {
			pObjTool = dynamic_cast<CObjTool*>(vecObj[i]);
			if (pObjTool)
				break;
		}
		assert(pObjTool);

		if (nullptr == pObjTool)
			break;
		pObjTool->OpenItemTool();

		break;
	}
		break;
	case ID_ITEM_LOAD: 
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
			pToolScene->LoadItemDialogBox(ofn.lpstrFile);

			wchar_t str[255] = L"Item File Load";
			MessageBox(CCore::GetInstance()->GetWndHandle(), str, L"Load", MB_OK);
		}
	}
		break;
	case ID_ITEM_SAVE: 
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
			pToolScene->SaveItem(ofn.lpstrFile);

			wchar_t str[255] = L"Item File Save";
			MessageBox(CCore::GetInstance()->GetWndHandle(), str, L"Save", MB_OK);
		}
		break;
	}

	// Object Tool
	case ID_OBJECT_TOOL: {
		// Modeless dialog
		if (!IsWindow(hWndObjectTool)) {
			hWndObjectTool = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_OBJECT_TOOL), hWnd, ObjectTool);
			ShowWindow(hWndObjectTool, SW_SHOW);
		}
		else
			SetFocus(hWndObjectTool);
		InvalidateRect(hWndObjectTool, nullptr, true);

		vector<CObject*>& vecObj = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::PREV);
		CObjTool* pObjTool = nullptr;
		for (int i = 0; vecObj.size(); ++i) {
			pObjTool = dynamic_cast<CObjTool*>(vecObj[i]);
			if (pObjTool)
				break;
		}
		assert(pObjTool);

		if (nullptr == pObjTool)
			break;
		pObjTool->OpenPlayerTool();
		break;
	}
	case ID_OBJECT_TOOL_SAVE: {
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
			pToolScene->SavePlayer(ofn.lpstrFile);

			wchar_t str[255] = L"Player File Save";
			MessageBox(CCore::GetInstance()->GetWndHandle(), str, L"Save", MB_OK);
		}
	}

		break;
	case ID_OBJECT_TOOL_LOAD: {
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
			pToolScene->LoadItemDialogBox(ofn.lpstrFile);

			wchar_t str[255] = L"Player File Load";
			MessageBox(CCore::GetInstance()->GetWndHandle(), str, L"Load", MB_OK);
		}
	}
		break;
		
	case ID_CHARACTER_TOOL:
	{
		// Modeless dialog
		if (!IsWindow(hWndItemTool)) {
			hWndItemTool = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_CHACTER_TOOL), hWnd, CharacterTool);
			ShowWindow(hWndItemTool, SW_SHOW);
		}
		else
			SetFocus(hWndItemTool);
		InvalidateRect(hWndItemTool, nullptr, true);
		break;
	}

	default:
		break;
	}
}

inline bool operator>(const TTilePos& _l, const TTilePos& _r) {
	if (_l.x > _r.x && _l.y > _r.y)
		return true;
	return false;
}