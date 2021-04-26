#include "stdafx.h"
#include "CScene.h"

#include "CCore.h"
#include <typeinfo>
#include "CCollisionManager.h"
#include "CPathManager.h"

#include "CObject.h"
#include "CCamera.h"
#include "CTileMap.h"
#include "CTile.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CGTA_Item.h"
#include "CGTA_Player.h"


CScene::CScene() :
	m_pTileMap(nullptr),
	m_pMainCamera(nullptr)
{
}

CScene::~CScene()
{
	DeleteAllObjects();
}

void CScene::AddObject(CObject* _obj)
{
	assert(nullptr != _obj);
	E_GroupType groupType = _obj->GetGroupType();
	m_vecObj[(UINT)groupType].push_back(_obj);
}

// 그룹에 있는 모든 오브젝트 삭제
void CScene::DeleteObjects(E_GroupType _eType)
{
	for (UINT i = 0; i < m_vecObj[(UINT)_eType].size(); ++i) {
		UINT j = i;
		assert(nullptr != m_vecObj[(UINT)_eType][i]);
		delete m_vecObj[(UINT)_eType][i];
	}
	m_vecObj[(UINT)_eType].clear();
}

void CScene::DeleteAllObjects()
{
	for (UINT i = 0; i < (UINT)E_GroupType::END; ++i)
		DeleteObjects((E_GroupType)i);
	m_pTileMap = nullptr;
}

CObject* CScene::FindObject(wstring _objectName)
{
	for (UINT i = 0; i < (UINT)E_GroupType::END; ++i) {
		for (UINT j = 0; j < m_vecObj[i].size(); ++j) {
			if (m_vecObj[i][j]->GetObjectName() == _objectName)
				return m_vecObj[i][j];
		}
	}
	return nullptr;
}

void CScene::PrevUpdate()
{
	for (UINT i = 0; i < (UINT)E_GroupType::END; ++i) {
		if (E_GroupType::TILE == (E_GroupType)i) {
			PrevUpdateTile();
			continue;
		}

		for (UINT j = 0; j < m_vecObj[i].size(); ++j) {
			if (!m_vecObj[i][j]->IsActive())
				continue;
			if (!m_vecObj[i][j]->IsDead())
				m_vecObj[i][j]->PrevUpdate();
		}
	}
}

void CScene::Update()
{
	for (UINT i = 0; i < (UINT)E_GroupType::END; ++i) {
		if (E_GroupType::TILE == (E_GroupType)i) {
			UpdateTile();
			continue;
		}

		for (UINT j = 0; j < m_vecObj[i].size(); ++j) {
			if (!m_vecObj[i][j]->IsActive())
				continue;
			if (!m_vecObj[i][j]->IsDead())
				m_vecObj[i][j]->Update();
		}
			
	}
}
void CScene::LateUpdate()
{
	for (UINT i = 0; i < (UINT)E_GroupType::END; ++i) {
		if (E_GroupType::TILE == (E_GroupType)i) {
			LateUpdateTile();
			continue;
		}

		for (UINT j = 0; j < m_vecObj[i].size(); ++j) {
			if (!m_vecObj[i][j]->IsActive())
				continue;
			if (!m_vecObj[i][j]->IsDead())
				m_vecObj[i][j]->LateUpdate();
		}
	}
}

void CScene::Render(HDC _hDC)
{
	for (UINT i = 0; i < (UINT)E_GroupType::END; ++i) {
		if (E_GroupType::TILE == (E_GroupType)i) {
			
			RenderTile(_hDC);
			continue;
		}
		vector<CObject*>::iterator iter = m_vecObj[i].begin();
		while (iter != m_vecObj[i].end()) {
			if (nullptr == (*iter)) {
				iter = m_vecObj[i].erase(iter);
				continue;
			}
			if ((*iter)->IsDead())
				iter = m_vecObj[i].erase(iter);
			else {
				if (!(*iter)->IsActive()) {
					++iter;
					continue;
				}
				if ((*iter)->IsRender())
					(*iter)->Render(_hDC);
				++iter;
			}
		}
	}
}

void CScene::PrevUpdateTile()
{
	if (nullptr == m_pTileMap)
		return;
	if (!m_pTileMap->IsActive())
		return;

	vector<CObject*>& vecObjs = GetObjects(E_GroupType::TILE);

	// 해상도
	POINT ptResolution = CCore::GetInstance()->GetResolution();

	// 렌더링할 카메라의 좌상단 인덱스
	Vector2 vLeftTopPos = MainCamera->GetScreenToWorldPosition(Vector2(0.f, 0.f));

	int iRowCnt = int(vLeftTopPos.y / TILE_SIZE);
	int iColCnt = int(vLeftTopPos.x / TILE_SIZE);

	// 좌상단 idx를 구한다.
	int iLeftTopIdx = iRowCnt * (int)m_pTileMap->GetCol() + iColCnt;

	// 화면에 보여줄 수 있는 rol와 col의 최대 개수
	int iMaxCol = ptResolution.x / TILE_SIZE + 1;
	int iMaxRow = ptResolution.y / TILE_SIZE + 1;

	// 타일의 col의 총 갯수에서 화면에 보여줄 컬럼 개수를 뺀 갯수가 화면에 보여줄 수 있는 col의 최대 갯수보다 작으면
	if ((int)m_pTileMap->GetCol() - iColCnt < iMaxCol) {
		iMaxCol = (int)m_pTileMap->GetCol() - iColCnt; // 화면에 보여줄 최대 col의 개수를 재설정.
	}

	if ((int)m_pTileMap->GetRow() - iRowCnt < iMaxRow) {
		iMaxRow = (int)m_pTileMap->GetRow() - iRowCnt;
	}

	for (int iRow = 0; iRow < iMaxRow; ++iRow) {
		for (int iCol = 0; iCol < iMaxCol; ++iCol) {
			int iResultIdx = iLeftTopIdx + m_pTileMap->GetCol() * iRow + iCol;
			if (iResultIdx < 0 || iResultIdx >= (int)m_pTileMap->GetCol() * (int)m_pTileMap->GetRow())
				continue;
			if (!vecObjs[iResultIdx]->IsActive())
				continue;
			vecObjs[iResultIdx]->PrevUpdate();
		}
	}
}

void CScene::UpdateTile()
{
	if (nullptr == m_pTileMap)
		return;
	if (!m_pTileMap->IsActive())
		return;

	vector<CObject*>& vecObjs = GetObjects(E_GroupType::TILE);

	// 해상도
	POINT ptResolution = CCore::GetInstance()->GetResolution();

	// 렌더링할 카메라의 좌상단 인덱스
	Vector2 vLeftTopPos = MainCamera->GetScreenToWorldPosition(Vector2(0.f, 0.f));

	int iRowCnt = int(vLeftTopPos.y / TILE_SIZE);
	int iColCnt = int(vLeftTopPos.x / TILE_SIZE);

	// 좌상단 idx를 구한다.
	int iLeftTopIdx = iRowCnt * (int)m_pTileMap->GetCol() + iColCnt;

	// 화면에 보여줄 수 있는 rol와 col의 최대 개수
	int iMaxCol = ptResolution.x / TILE_SIZE + 1;
	int iMaxRow = ptResolution.y / TILE_SIZE + 1;

	// 타일의 col의 총 갯수에서 화면에 보여줄 컬럼 개수를 뺀 갯수가 화면에 보여줄 수 있는 col의 최대 갯수보다 작으면
	if ((int)m_pTileMap->GetCol() - iColCnt < iMaxCol) {
		iMaxCol = (int)m_pTileMap->GetCol() - iColCnt; // 화면에 보여줄 최대 col의 개수를 재설정.
	}

	if ((int)m_pTileMap->GetRow() - iRowCnt < iMaxRow) {
		iMaxRow = (int)m_pTileMap->GetRow() - iRowCnt;
	}

	for (int iRow = 0; iRow < iMaxRow; ++iRow) {
		for (int iCol = 0; iCol < iMaxCol; ++iCol) {
			int iResultIdx = iLeftTopIdx + (int)m_pTileMap->GetCol() * iRow + iCol;
			if (iResultIdx < 0 || iResultIdx >= (int)m_pTileMap->GetCol() * (int)m_pTileMap->GetRow())
				continue;
			if (!vecObjs[iResultIdx]->IsActive())
				continue;
			vecObjs[iResultIdx]->Update();
		}
	}
}

void CScene::LateUpdateTile()
{
	if (nullptr == m_pTileMap)
		return;
	if (!m_pTileMap->IsActive())
		return;

	vector<CObject*>& vecObjs = GetObjects(E_GroupType::TILE);

	// 해상도
	POINT ptResolution = CCore::GetInstance()->GetResolution();

	// 렌더링할 카메라의 좌상단 인덱스
	Vector2 vLeftTopPos = MainCamera->GetScreenToWorldPosition(Vector2(0.f, 0.f));

	int iRowCnt = int(vLeftTopPos.y / TILE_SIZE);
	int iColCnt = int(vLeftTopPos.x / TILE_SIZE);

	// 좌상단 idx를 구한다.
	int iLeftTopIdx = iRowCnt * (int)m_pTileMap->GetCol() + iColCnt;

	// 화면에 보여줄 수 있는 rol와 col의 최대 개수
	int iMaxCol = ptResolution.x / TILE_SIZE + 1;
	int iMaxRow = ptResolution.y / TILE_SIZE + 1;

	// 타일의 col의 총 갯수에서 화면에 보여줄 컬럼 개수를 뺀 갯수가 화면에 보여줄 수 있는 col의 최대 갯수보다 작으면
	if ((int)m_pTileMap->GetCol() - iColCnt < iMaxCol) {
		iMaxCol = (int)m_pTileMap->GetCol() - iColCnt; // 화면에 보여줄 최대 col의 개수를 재설정.
	}

	if ((int)m_pTileMap->GetRow() - iRowCnt < iMaxRow) {
		iMaxRow = (int)m_pTileMap->GetRow() - iRowCnt;
	}

	for (int iRow = 0; iRow < iMaxRow; ++iRow) {
		for (int iCol = 0; iCol < iMaxCol; ++iCol) {
			int iResultIdx = iLeftTopIdx + (int)m_pTileMap->GetCol() * iRow + iCol;
			if (iResultIdx < 0 || iResultIdx >= (int)m_pTileMap->GetCol() * (int)m_pTileMap->GetRow())
				continue;
			if (!vecObjs[iResultIdx]->IsActive())
				continue;
			vecObjs[iResultIdx]->LateUpdate();
		}
	}
}

void CScene::RenderTile(HDC _hDC)
{
	if (nullptr == m_pTileMap)
		return;
	if (!m_pTileMap->IsActive())
		return;

	vector<CObject*>& vecObjs = GetObjects(E_GroupType::TILE);

	// 해상도
	POINT ptResolution = CCore::GetInstance()->GetResolution();

	// 렌더링할 카메라의 좌상단 인덱스
	Vector2 vLeftTopPos = MainCamera->GetScreenToWorldPosition(Vector2(0.f, 0.f));

	int iRowCnt = int(vLeftTopPos.y / TILE_SIZE);
	int iColCnt = int(vLeftTopPos.x / TILE_SIZE);

	// 좌상단 idx를 구한다.
	int iLeftTopIdx = iRowCnt * (int)m_pTileMap->GetCol() + iColCnt;

	// 화면에 보여줄 수 있는 rol와 col의 최대 개수
	int iMaxCol = ptResolution.x / TILE_SIZE + 1;
	int iMaxRow = ptResolution.y / TILE_SIZE + 1;

	// 타일의 col의 총 갯수에서 화면에 보여줄 컬럼 개수를 뺀 갯수가 화면에 보여줄 수 있는 col의 최대 갯수보다 작으면
	if ((int)m_pTileMap->GetCol() - iColCnt < iMaxCol) { 
		iMaxCol = (int)m_pTileMap->GetCol() - iColCnt; // 화면에 보여줄 최대 col의 개수를 재설정.
	}

	if ((int)m_pTileMap->GetRow() - iRowCnt < iMaxRow) {
		iMaxRow = (int)m_pTileMap->GetRow() - iRowCnt;
	}


	for (int iRow = 0; iRow < iMaxRow; ++iRow) {
		for (int iCol = 0; iCol < iMaxCol; ++iCol) {
			int colSize = (int)m_pTileMap->GetCol();
			int iResultIdx = iLeftTopIdx + colSize * iRow + iCol;
			if (iResultIdx < 0 || iResultIdx >= (int)m_pTileMap->GetCol() * (int)m_pTileMap->GetRow())
				continue;
			if (!vecObjs[iResultIdx]->IsActive())
				continue;
			if (!vecObjs[iResultIdx]->IsRender())
				continue;

			vecObjs[iResultIdx]->Render(_hDC);
		}
	}
}

void CScene::LoadTile(wstring _strRelativePath)
{
	wstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativePath;
	LoadTileDialogBox(strFilePath);
	m_pTileMap->OptimizationTileCollider(); // 최적화
}

void CScene::LoadTileDialogBox(wstring _strPath)
{
	wstring strFilePath = _strPath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	if (nullptr == pFile) {
		assert(pFile && L"경로 이상함. 타일 로드 실패");
		return;
	}

	int iRow = 0;
	int iCol = 0;
	fread(&iRow, sizeof(int), 1, pFile);
	fread(&iCol, sizeof(int), 1, pFile);

	DeleteObjects(E_GroupType::TILE);
	assert(m_pTileMap && L"Tile Map이 없음.");
	m_pTileMap->CreateTileGrid((UINT)iRow, (UINT)iCol);

	vector<CObject*>& vecTiles = GetObjects(E_GroupType::TILE);
	for (int i = 0; i < iRow * iCol; ++i) {
		CTile* pTile = dynamic_cast<CTile*>(vecTiles[i]);
		if (pTile) {
			pTile->Load(pFile);
		}

	}
	fclose(pFile);
}


void CScene::LoadItem(wstring _strRelativePath)
{
	wstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativePath;
	LoadItemDialogBox(strFilePath);
}

void CScene::LoadItemDialogBox(wstring _strPath)
{
	wstring strFilePath = _strPath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	if (nullptr == pFile) {
		assert(pFile && L"경로 이상함. 아이템 로드 실패");
		return;
	}

	int iCnt = 0;
	fread(&iCnt, sizeof(int), 1, pFile);

	DeleteObjects(E_GroupType::ITEM);

	for (int i = 0; i < iCnt; ++i) {
		CGTA_Item* pItem = new CGTA_Item(E_GroupType::ITEM);
		pItem->Init();
		pItem->Load(pFile);
		CreateObject(pItem);
	}
	fclose(pFile);
}

void CScene::LoadPlayer(wstring _strRelativePath)
{
	wstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativePath;
	LoadPlayerDialogBox(strFilePath);
}

void CScene::LoadPlayerDialogBox(wstring _strPath)
{
	wstring strFilePath = _strPath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	if (nullptr == pFile) {
		assert(pFile && L"경로 이상함. 플레이어 로드 실패");
		return;
	}

	int iCnt = 0;
	fread(&iCnt, sizeof(int), 1, pFile);

	DeleteObjects(E_GroupType::PLAYER);

	for (int i = 0; i < iCnt; ++i) {
		CGTA_Player* pPlayer = new CGTA_Player(E_GroupType::PLAYER);
		pPlayer->Init();
		pPlayer->Load(pFile);
		AddObject(pPlayer);
	}
	fclose(pFile);
}

void CScene::LoadAll()
{
	wstring strFilePath = CPathManager::GetInstance()->GetContentPath();
	// Path : MetaData path
	// load player data
	LoadPlayer(STR_FILE_PATH_GTA_Player_Save);
	
	// load vehicle data

	// load item data
	LoadItem(STR_FILE_PATH_GTA_Item_Save);


	// load Save data
	
}