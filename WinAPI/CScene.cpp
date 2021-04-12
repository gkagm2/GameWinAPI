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

// �׷쿡 �ִ� ��� ������Ʈ ����
void CScene::DeleteObjects(E_GroupType _eType)
{
	for (UINT i = 0; i < m_vecObj[(UINT)_eType].size(); ++i) {
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
			if (!(*iter)->IsActive()) {
				++iter;
				continue;
			}

			if ((*iter)->IsDead())
				iter = m_vecObj[i].erase(iter);
			else {
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

	// �ػ�
	POINT ptResolution = CCore::GetInstance()->GetResolution();

	// �������� ī�޶��� �»�� �ε���
	Vector2 vLeftTopPos = MainCamera->GetScreenToWorldPosition(Vector2(0.f, 0.f));

	int iRowCnt = int(vLeftTopPos.y / TILE_SIZE);
	int iColCnt = int(vLeftTopPos.x / TILE_SIZE);

	// �»�� idx�� ���Ѵ�.
	int iLeftTopIdx = iRowCnt * (int)m_pTileMap->GetCol() + iColCnt;

	// ȭ�鿡 ������ �� �ִ� rol�� col�� �ִ� ����
	int iMaxCol = ptResolution.x / TILE_SIZE + 1;
	int iMaxRow = ptResolution.y / TILE_SIZE + 1;

	// Ÿ���� col�� �� �������� ȭ�鿡 ������ �÷� ������ �� ������ ȭ�鿡 ������ �� �ִ� col�� �ִ� �������� ������
	if (m_pTileMap->GetCol() - iColCnt < iMaxCol) {
		iMaxCol = m_pTileMap->GetCol() - iColCnt; // ȭ�鿡 ������ �ִ� col�� ������ �缳��.
	}

	if (m_pTileMap->GetRow() - iRowCnt < iMaxRow) {
		iMaxRow = m_pTileMap->GetRow() - iRowCnt;
	}

	for (int iRow = 0; iRow < iMaxRow; ++iRow) {
		for (int iCol = 0; iCol < iMaxCol; ++iCol) {
			int iResultIdx = iLeftTopIdx + m_pTileMap->GetCol() * iRow + iCol;
			if (iResultIdx < 0 || iResultIdx >= m_pTileMap->GetCol() * m_pTileMap->GetRow())
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

	// �ػ�
	POINT ptResolution = CCore::GetInstance()->GetResolution();

	// �������� ī�޶��� �»�� �ε���
	Vector2 vLeftTopPos = MainCamera->GetScreenToWorldPosition(Vector2(0.f, 0.f));

	int iRowCnt = int(vLeftTopPos.y / TILE_SIZE);
	int iColCnt = int(vLeftTopPos.x / TILE_SIZE);

	// �»�� idx�� ���Ѵ�.
	int iLeftTopIdx = iRowCnt * (int)m_pTileMap->GetCol() + iColCnt;

	// ȭ�鿡 ������ �� �ִ� rol�� col�� �ִ� ����
	int iMaxCol = ptResolution.x / TILE_SIZE + 1;
	int iMaxRow = ptResolution.y / TILE_SIZE + 1;

	// Ÿ���� col�� �� �������� ȭ�鿡 ������ �÷� ������ �� ������ ȭ�鿡 ������ �� �ִ� col�� �ִ� �������� ������
	if (m_pTileMap->GetCol() - iColCnt < iMaxCol) {
		iMaxCol = m_pTileMap->GetCol() - iColCnt; // ȭ�鿡 ������ �ִ� col�� ������ �缳��.
	}

	if (m_pTileMap->GetRow() - iRowCnt < iMaxRow) {
		iMaxRow = m_pTileMap->GetRow() - iRowCnt;
	}

	for (int iRow = 0; iRow < iMaxRow; ++iRow) {
		for (int iCol = 0; iCol < iMaxCol; ++iCol) {
			int iResultIdx = iLeftTopIdx + m_pTileMap->GetCol() * iRow + iCol;
			if (iResultIdx < 0 || iResultIdx >= m_pTileMap->GetCol() * m_pTileMap->GetRow())
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

	// �ػ�
	POINT ptResolution = CCore::GetInstance()->GetResolution();

	// �������� ī�޶��� �»�� �ε���
	Vector2 vLeftTopPos = MainCamera->GetScreenToWorldPosition(Vector2(0.f, 0.f));

	int iRowCnt = int(vLeftTopPos.y / TILE_SIZE);
	int iColCnt = int(vLeftTopPos.x / TILE_SIZE);

	// �»�� idx�� ���Ѵ�.
	int iLeftTopIdx = iRowCnt * (int)m_pTileMap->GetCol() + iColCnt;

	// ȭ�鿡 ������ �� �ִ� rol�� col�� �ִ� ����
	int iMaxCol = ptResolution.x / TILE_SIZE + 1;
	int iMaxRow = ptResolution.y / TILE_SIZE + 1;

	// Ÿ���� col�� �� �������� ȭ�鿡 ������ �÷� ������ �� ������ ȭ�鿡 ������ �� �ִ� col�� �ִ� �������� ������
	if (m_pTileMap->GetCol() - iColCnt < iMaxCol) {
		iMaxCol = m_pTileMap->GetCol() - iColCnt; // ȭ�鿡 ������ �ִ� col�� ������ �缳��.
	}

	if (m_pTileMap->GetRow() - iRowCnt < iMaxRow) {
		iMaxRow = m_pTileMap->GetRow() - iRowCnt;
	}

	for (int iRow = 0; iRow < iMaxRow; ++iRow) {
		for (int iCol = 0; iCol < iMaxCol; ++iCol) {
			int iResultIdx = iLeftTopIdx + m_pTileMap->GetCol() * iRow + iCol;
			if (iResultIdx < 0 || iResultIdx >= m_pTileMap->GetCol() * m_pTileMap->GetRow())
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

	// �ػ�
	POINT ptResolution = CCore::GetInstance()->GetResolution();

	// �������� ī�޶��� �»�� �ε���
	Vector2 vLeftTopPos = MainCamera->GetScreenToWorldPosition(Vector2(0.f, 0.f));

	int iRowCnt = int(vLeftTopPos.y / TILE_SIZE);
	int iColCnt = int(vLeftTopPos.x / TILE_SIZE);

	// �»�� idx�� ���Ѵ�.
	int iLeftTopIdx = iRowCnt * (int)m_pTileMap->GetCol() + iColCnt;

	// ȭ�鿡 ������ �� �ִ� rol�� col�� �ִ� ����
	int iMaxCol = ptResolution.x / TILE_SIZE + 1;
	int iMaxRow = ptResolution.y / TILE_SIZE + 1;

	// Ÿ���� col�� �� �������� ȭ�鿡 ������ �÷� ������ �� ������ ȭ�鿡 ������ �� �ִ� col�� �ִ� �������� ������
	if (m_pTileMap->GetCol() - iColCnt < iMaxCol) { 
		iMaxCol = m_pTileMap->GetCol() - iColCnt; // ȭ�鿡 ������ �ִ� col�� ������ �缳��.
	}

	if (m_pTileMap->GetRow() - iRowCnt < iMaxRow) {
		iMaxRow = m_pTileMap->GetRow() - iRowCnt;
	}


	for (int iRow = 0; iRow < iMaxRow; ++iRow) {
		for (int iCol = 0; iCol < iMaxCol; ++iCol) {
			int colSize = m_pTileMap->GetCol();
			int iResultIdx = iLeftTopIdx + colSize * iRow + iCol;
			if (iResultIdx < 0 || iResultIdx >= m_pTileMap->GetCol() * m_pTileMap->GetRow())
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
}

void CScene::LoadTileDialogBox(wstring _strPath)
{
	wstring strFilePath = _strPath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	if (nullptr == pFile) {
		assert(pFile && L"��� �̻���. Ÿ�� �ε� ����");
		return;
	}

	int iRow = 0;
	int iCol = 0;
	fread(&iRow, sizeof(int), 1, pFile);
	fread(&iCol, sizeof(int), 1, pFile);

	DeleteObjects(E_GroupType::TILE);
	assert(m_pTileMap && L"Tile Map�� ����.");
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

void CScene::LoadAll(wstring _strPath)
{
	wstring strFilePath = _strPath;
	// load player data


	// load vehicle data

	// load item data

	// load Save data
}