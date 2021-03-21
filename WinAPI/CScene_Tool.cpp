#include "stdafx.h"

#include "resource.h"
#include "CCore.h"

#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CPathManager.h"

#include "CScene.h"
#include "CScene_Tool.h"

#include "CTileMap.h"
#include "CTile.h"

#include "CObject.h"
#include "CCamera.h"


CScene_Tool::CScene_Tool() :
	m_hMenu(nullptr),
	m_pTileMap(nullptr)
{
	// �޴��� ����
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCE(IDC_WINAPI));
	//errno_t err = GetLastError();
}

CScene_Tool::~CScene_Tool()
{
	DestroyMenu(m_hMenu);
}

void CScene_Tool::Start()
{
	// �޴� �߰�
	SetMenu(CCore::GetInstance()->GetWndHandle(), m_hMenu);

	// �޴��� �߰��� ���¿��� ������ ������ �缳�� (�ػ� ����)
	CCore::GetInstance()->ResizeWindowScreen(CCore::GetInstance()->GetResolution(), true); 

	// ī�޶� Look ����
	POINT ptResolution = CCore::GetInstance()->GetResolution();
	ptResolution.x /= 2;
	ptResolution.y /= 2;
	CCamera::GetInstance()->SetLookAt(Vector2((float)ptResolution.x, (float)ptResolution.y));

	// Ÿ�� ���� ����
	m_pTileMap = new CTileMap(E_GroupType::TILEMAP);
	m_pTileMap->SetPosition(0, 0, 0);
	m_pTileMap->CreateTileGrid(10, 10);
	AddObject(m_pTileMap);
}

void CScene_Tool::Update()
{
	CScene::Update();
	MouseClick();
}

void CScene_Tool::End()
{
	// �޴� ����
	SetMenu(CCore::GetInstance()->GetWndHandle(), nullptr);

	// �޴��� ������ ���¿��� ������ ������ �缳�� (�ػ� ����)
	CCore::GetInstance()->ResizeWindowScreen(CCore::GetInstance()->GetResolution(), false);

	DeleteAllObjects();
}

void CScene_Tool::MouseClick()
{
	if (InputKeyPress(E_Key::LBUTTON)) {
		Vector2 vMousePos = MousePosition;

		vector<CObject*>& pTiles = GetObjects(E_GroupType::TILE);

		// ������ üũ�Ѵ�.
		Vector2 vClickPos = CCamera::GetInstance()->GetScreenToWorldPosition(vMousePos);

		int iClickedCol = int(vClickPos.x / CTile::GetTileSize());
		int iClickedRow = int(vClickPos.y / CTile::GetTileSize());

		if (m_pTileMap) {
			UINT wid = m_pTileMap->GetTileMapWidth();
			UINT hegith = m_pTileMap->GetTileMapHeight();

			// ���� ���̸�
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

void CScene_Tool::SaveTile(wstring _strRelativePath)
{
	vector<CObject*>& vecTiles = GetObjects(E_GroupType::TILE);
	wstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativePath;

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

void CScene_Tool::LoadTile(wstring _strRelativePath)
{
	wstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	if (nullptr == pFile) {
		return;
	}

	int iRow = 0;
	int iCol = 0;
	fread(&iRow, sizeof(int), 1, pFile);
	fread(&iCol, sizeof(int), 1, pFile);

	DeleteObjects(E_GroupType::TILE);
	m_pTileMap->CreateTileGrid((UINT)iRow, (UINT)iCol);

	vector<CObject*>& vecTiles = GetObjects(E_GroupType::TILE);
	for (int i = 0; i < iRow * iCol; ++i) {
		CTile* pTile = dynamic_cast<CTile*>(vecTiles[i]);
		if (pTile)
			pTile->Load(pFile);
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
					pToolScene->DeleteObjects(E_GroupType::TILE); // �ʱ�ȭ
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

	return INT_PTR(FALSE);
}