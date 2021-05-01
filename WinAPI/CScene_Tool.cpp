#include "stdafx.h"

#include "resource.h"
#include "CCore.h"

#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CPathManager.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"

#include "CScene.h"
#include "CScene_Tool.h"

#include "CTileMap.h"
#include "CTile.h"

#include "CObject.h"
#include "CCamera.h"

#include "CUI.h"
#include "CPanelUI.h"
#include "CTileUI.h"
#include "CTileToolPanelUI.h"
#include "CCamera_Tool.h"

#include "CGTA_Player.h"
#include "CGTA_PoliceCar.h"
#include "CColliderRect.h"
#include "CTexture.h"
#include "CObjTool.h"

#include "CDragScreen.h"

#include "CDebug.h"
#include "CGTA_Item.h"

#include "CPathFinding.h"
#include "CAITestObj.h"
#include "CLookAtTestObj.h"

CScene_Tool::CScene_Tool() :
	m_hMenu(nullptr)
{
	// 메뉴바 생성
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCE(IDC_WINAPI));
	//errno_t err = GetLastError();
}

CScene_Tool::~CScene_Tool()
{
	DestroyMenu(m_hMenu);
}

void CScene_Tool::Start()
{
	// 메뉴 추가
	SetMenu(CCore::GetInstance()->GetWndHandle(), m_hMenu);

	// 메뉴를 추가된 상태에서 윈도우 사이즈 재설정 (해상도 유지)
	CCore::GetInstance()->ResizeWindowScreen(CCore::GetInstance()->GetResolution(), true); 

	// 카메라 Look 설정
	POINT ptResolution = CCore::GetInstance()->GetResolution();
	ptResolution.x /= 2;
	ptResolution.y /= 2;

	// 툴 전용 카메라 생성
	CCamera_Tool* pCamera = new CCamera_Tool(E_GroupType::MAIN_CAMERA);
	pCamera->SetMainCamera();
	pCamera->SetLookAt(Vector2((float)ptResolution.x, (float)ptResolution.y));
	AddObject((CObject*)pCamera);
	
	// 타일 격자 생성
	m_pTileMap = new CTileMap(E_GroupType::TILEMAP);
	m_pTileMap->SetPosition(0, 0, 0);
	m_pTileMap->CreateTileGrid(10, 10);
	AddObject(m_pTileMap);
	
	// 툴 모음
	CObjTool* pObjTool = new CObjTool(E_GroupType::PREV);
	pObjTool->Init();
	AddObject(pObjTool);

	LoadAll();

	// Select Controller에서는 오브젝트들을 드래그해서 선택할 수 있다.
	// Tile setting 모드와
	// item setting 모드와
	// Vehicle setting 모드가 있다.

	// Tile setting 모드는 타일을 조작할 수 있다.
	// item setting 모드는 item의 위치를 조작할 수 있다.
	// Vehicle setting 모드는 차량의 위치를 조작할 수 있다.
	// Player setting 모드는 Player의 위치를 조작할 수 있다.
	// Citizen setting 모드는 Citizen의 랜덤 생성 위치를 조작할 수 있다.

	CCollisionManager::GetInstance()->ClearAllCollisionGroup();
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::VEHICLE, E_GroupType::TILE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::VEHICLE, E_GroupType::PLAYER, true);
}

void CScene_Tool::Update()
{
	CScene::Update();
	if (InputKeyPress(E_Key::F4))
		CSceneManager::GetInstance()->ChangeScene(E_SceneType::GTA_IN);
}

void CScene_Tool::End()
{
	// 메뉴 해제
	SetMenu(CCore::GetInstance()->GetWndHandle(), nullptr);

	// 메뉴를 해제된 상태에서 윈도우 사이즈 재설정 (해상도 유지)
	CCore::GetInstance()->ResizeWindowScreen(CCore::GetInstance()->GetResolution(), false);

	DeleteAllObjects();
	CResourceManager::GetInstance()->InitAllSounds();
}

void CScene_Tool::LoadAll()
{
	LoadTile(STR_FILE_PATH_GTA_TILES_Save);
	//LoadPlayer(STR_FILE_PATH_GTA_Player_Save);
	LoadItem(STR_FILE_PATH_GTA_Item_Save);
}

void CScene_Tool::SaveTile(wstring _strPath)
{
	vector<CObject*>& vecTiles = GetObjects(E_GroupType::TILE);
	//wstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativePath;
	wstring strFilePath = _strPath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	if (nullptr == pFile) {
		assert(pFile);
		return;
	}
		
	int iRow = m_pTileMap->GetRow();
	int iCol = m_pTileMap->GetCol();
	
	fwrite(&iRow, sizeof(int), 1, pFile);
	fwrite(&iCol, sizeof(int), 1, pFile);

	for (UINT i = 0; i < vecTiles.size(); ++i) {
		CTile* pTile = dynamic_cast<CTile*>(vecTiles[i]);
		if (pTile)
			pTile->Save(pFile);
	}

	fclose(pFile);
}

void CScene_Tool::SaveItem(wstring _strPath)
{
	vector<CObject*>& vecItems = GetObjects(E_GroupType::ITEM);
	//wstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strPath;
	wstring strFilePath = _strPath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	if (nullptr == pFile) {
		assert(pFile);
		return;
	}

	int iCnt = (int)vecItems.size();
	fwrite(&iCnt, sizeof(int), 1, pFile);

	for (UINT i = 0; i < vecItems.size(); ++i) {
		CGTA_Item* pItem = dynamic_cast<CGTA_Item*>(vecItems[i]);
		if (pItem)
			pItem->Save(pFile);
	}

	fclose(pFile);
}

void CScene_Tool::SavePlayer(wstring _strPath)
{
	vector<CObject*>& vecPlayer = GetObjects(E_GroupType::PLAYER);
	//wstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strPath;
	wstring strFilePath = _strPath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	if (nullptr == pFile) {
		assert(pFile);
		return;
	}

	int iCnt = (int)vecPlayer.size();
	fwrite(&iCnt, sizeof(int), 1, pFile);

	for (UINT i = 0; i < vecPlayer.size(); ++i) {
		CGTA_Player* pPlayer = dynamic_cast<CGTA_Player*>(vecPlayer[i]);
		if (pPlayer)
			pPlayer->Save(pFile);
	}

	fclose(pFile);
}

INT_PTR CreateTileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND: 
		if (LOWORD(wParam) == IDOK) {
			int iCol = GetDlgItemInt(hWnd, IDC_EDIT_COL, nullptr, true);
			int iRow = GetDlgItemInt(hWnd, IDC_EDIT_ROW, nullptr, true);
			if (iCol > 0 && iRow > 0) {
				CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(CSceneManager::GetInstance()->GetCurScene());
				if (pToolScene) {
					pToolScene->DeleteObjects(E_GroupType::TILE); // 초기화
					pToolScene->GetTileMap()->CreateTileGrid((UINT)iRow, (UINT)iCol);
				}
				EndDialog(hWnd, LOWORD(wParam));
			}
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL) {
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CharacterTool(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDC_Load_BUTTON2) {
			MessageBox(CCore::GetInstance()->GetWndHandle(), L"GOOD", L"TEST", MB_OK);
		}

		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}

// Item Tool
INT_PTR ItemTool(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	{
		vector<CObject*>& vecObj = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::PREV);

		CObjTool* pObjTool = nullptr;
		int i = -1;
		for (int i = 0; i < vecObj.size(); ++i) {
			pObjTool = dynamic_cast<CObjTool*>(vecObj[i]);
			if (nullptr != pObjTool)
				break;
		}
		assert(pObjTool && L"objTool이 null임");
		// TODO : 리스트로 바꾸기
		if (LOWORD(wParam) == IDC_CREATE_ROCKETLAUNCHER_BUTTON) {
			pObjTool->CreateWeaponItem(E_WeaponType::ROCKET_LAUNCHER);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_CREATE_PISTOL_BUTTON) {
			pObjTool->CreateWeaponItem(E_WeaponType::PISTOL);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_CREATE_SHOTGUN_BUTTON) {
			pObjTool->CreateWeaponItem(E_WeaponType::SHOTGUN);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_CREATE_SUBMACHINEGUN_BUTTON) {
			pObjTool->CreateWeaponItem(E_WeaponType::SUBMACHINE_GUN);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_INFO_UPDATE_BUTTON) {
			pObjTool->UpdateItemInfo();
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL) {
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	}
		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}

// Object Tool
INT_PTR ObjectTool(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	{
		vector<CObject*>& vecObj = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::PREV);

		CObjTool* pObjTool = nullptr;
		int i = -1;
		for (int i = 0; i < vecObj.size(); ++i) {
			pObjTool = dynamic_cast<CObjTool*>(vecObj[i]);
			if (nullptr != pObjTool)
				break;
		}
		assert(pObjTool && L"objTool이 null임");
		if (LOWORD(wParam) == IDC_SET_POSITION_TO_CENTER_BUTTON2) {
			pObjTool->SetPositionToCenter();
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL) {
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}
