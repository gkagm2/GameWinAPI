#include "stdafx.h"

#include "resource.h"
#include "CCore.h"

#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CPathManager.h"
#include "CUIManager.h"
#include "CResourceManager.h"

#include "CScene.h"
#include "CScene_Tool.h"

#include "CTileMap.h"
#include "CTile.h"

#include "CObject.h"
#include "CCamera.h"

#include "CUI.h"
#include "CPanelUI.h"
#include "CCamera_Tool.h"

#include "CGTA_Player.h"
#include "CGTA_PoliceCar.h"
#include "CColliderRect.h"

#include "CDebug.h"


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


	// Character 모음
	// Player
	CGTA_Player* pPlayer = new CGTA_Player(E_GroupType::PLAYER);
	pPlayer->SetObjectName(L"Player");
	pPlayer->Init();
	AddObject(pPlayer);

	// Vehicle
	CGTA_PoliceCar* pPoliceCar = new CGTA_PoliceCar(E_GroupType::VEHICLE);
	pPoliceCar->SetObjectName(L"Police Car");
	pPoliceCar->Init();
	AddObject(pPoliceCar);


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
	
	// UI 생성
	/*
	CPanelUI* pUI = new CPanelUI(E_GroupType::UI);
	pUI->Init();
	pUI->SetPosition(250, 150, 0);
	AddObject(pUI);
	
	CPanelUI* copyUI = pUI->Clone();
	copyUI->SetPosition(500, 500, 0);
	AddObject(copyUI);
	*/
}

void CScene_Tool::Update()
{
	CScene::Update();
	MouseClick();
}

void CScene_Tool::End()
{
	// 메뉴 해제
	SetMenu(CCore::GetInstance()->GetWndHandle(), nullptr);

	// 메뉴를 해제된 상태에서 윈도우 사이즈 재설정 (해상도 유지)
	CCore::GetInstance()->ResizeWindowScreen(CCore::GetInstance()->GetResolution(), false);

	DeleteAllObjects();
}

void CScene_Tool::MouseClick()
{
	if (InputKeyPress(E_Key::LBUTTON)) {
		Vector2 vMousePos = MousePosition;

		vector<CObject*>& pTiles = GetObjects(E_GroupType::TILE);

		// 영역을 체크한다.
		Vector2 vClickPos = MainCamera->GetScreenToWorldPosition(vMousePos);

		int iClickedCol = int(vClickPos.x / CTile::GetTileSize());
		int iClickedRow = int(vClickPos.y / CTile::GetTileSize());

		if (m_pTileMap) {
			UINT wid = m_pTileMap->GetTileMapWidth();
			UINT hegith = m_pTileMap->GetTileMapHeight();

			// 영역 밖이면
			if (vClickPos.x < m_pTileMap->GetPosition().x ||
				vClickPos.x > m_pTileMap->GetPosition().x + m_pTileMap->GetTileMapWidth() ||
				vClickPos.y < m_pTileMap->GetPosition().y ||
				vClickPos.y > m_pTileMap->GetPosition().y + m_pTileMap->GetTileMapHeight()) {
				return;
			}

			int iClickedIdx = iClickedRow * m_pTileMap->GetCol() + iClickedCol;
			CTile* pTile = dynamic_cast<CTile*>(pTiles[iClickedIdx]);
			if (pTile) {
				int iImgIdx = pTile->GetTileIdx();
				pTile->SetTile(iImgIdx + 1);
			}
		}
	}
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

INT_PTR CALLBACK CharacterTool(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDC_Load_BUTTON2) {
			MessageBox(CCore::GetInstance()->GetWndHandle(), L"GOOD", L"TEST", MB_OK);
		}

		break;
	case WM_CLOSE:
		DestroyWindow(hDlg);
		break;
	}
	return (INT_PTR)FALSE;
}