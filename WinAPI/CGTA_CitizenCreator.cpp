#include "stdafx.h"
#include "CGTA_CitizenCreator.h"
#include "CGTA_Citizen.h"
#include "CCamera2D.h"
#include "CCore.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CTile.h"
#include "CTileMap.h"
#include "CKeyManager.h"
#include "CObject.h"
#include "CDebug.h"
#include "CTimeManager.h"
#include "CCollider.h"
#include "CColliderRect.h"

CGTA_CitizenCreator::CGTA_CitizenCreator(E_GroupType e_GroupType) :
	CObject(e_GroupType),
	m_iCreateMargin(3),
	m_pCitizenObjPrefab(nullptr),
	m_pCamera(nullptr),
	m_iCurCitizenCnt(0),
	m_imaxCitizenCnt(80),
	m_fCreateCoolTime(0.f),
	m_fMaxCreateCoolTime(7.f),
	m_fDeleteCoolTime(0.f),
	m_fMaxDeleteCoolTime(3.f)
{
}

CGTA_CitizenCreator::~CGTA_CitizenCreator()
{
	if (m_pCitizenObjPrefab)
		delete m_pCitizenObjPrefab;
}

void CGTA_CitizenCreator::Init()
{
	m_pCitizenObjPrefab = new  CGTA_Citizen(E_GroupType::CITIZEN);
	m_pCitizenObjPrefab->SetObjectName(L"Citizen");
	m_pCitizenObjPrefab->Init();

	// TODO : 오브젝트 풀링 방식으로 바꾸는중
	for (int i = 0; i < m_imaxCitizenCnt; ++i) {
		CGTA_Citizen* pCitizen = m_pCitizenObjPrefab->Clone();
		pCitizen->SetActive(false);
		CreateObject(pCitizen);
	}
}

void CGTA_CitizenCreator::Update()
{
	// TODO : 오브젝트 풀링 방식으로 바꾸기

	// 주기적으로 생성
	m_fCreateCoolTime += DeltaTime;
	if (m_fCreateCoolTime >= m_fMaxCreateCoolTime) {

		// 씬에 있는 모든 타일들을 불러옴
		vector<CObject*>& vecTiles = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);

		// 타일맵을 가져옴
		CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
		if (nullptr == pTileMap) {
			assert(pTileMap);
			return;
		}

		// 카메라 바깥의 영역을 가져온다.
		POINT iResolution = CCore::GetInstance()->GetResolution();
		Vector3 vLeftTopWorldPos = m_pCamera->GetScreenToWorldPosition(Vector3(0.f, 0.f, 0.f));
		Vector3 vRightBottomWorldPos = m_pCamera->GetScreenToWorldPosition(Vector3((float)iResolution.x, (float)iResolution.y, 0));

		int iCamLTCol = (int)(vLeftTopWorldPos.x / TILE_SIZE);
		int iCamLTRow = (int)(vLeftTopWorldPos.y / TILE_SIZE);
		int iCamRBCol = (int)(vRightBottomWorldPos.x / TILE_SIZE);
		int iCamRBRow = (int)(vRightBottomWorldPos.y / TILE_SIZE);

		int iLTCol;
		int iLTRow;
		int iRBCol;
		int iRBRow;
		GetEnableCreateArea(iLTCol, iLTRow, iRBCol, iRBRow);

		vector<CObject*>& vecCitizen = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::CITIZEN);

		for (int iRow = iLTRow; iRow <= iRBRow; ++iRow) {
			for (int iCol = iLTCol; iCol <= iRBCol; ++iCol) {
				CTile* pTile = dynamic_cast<CTile*>(vecTiles[pTileMap->GetCol() * iRow + iCol]);
				if (nullptr == pTile)
					continue;
				if (m_iCurCitizenCnt > m_imaxCitizenCnt)
					break;
				if (iCol < iCamLTCol || iRow < iCamLTRow || iCol > iCamRBCol || iRow > iCamRBRow)
					continue;

				if (E_TileType::Road == pTile->GetTileType() || E_TileType::Sidewalk == pTile->GetTileType()) {
					// 시민을 생성.
					CGTA_Citizen* m_pClone = m_pCitizenObjPrefab->Clone();
					m_pClone->SetActive(true);

					// 시민 위치 설정.
					Vector3 vRespawnPos = { iCol * TILE_SIZE + TILE_SIZE * 0.5f, iRow * TILE_SIZE + TILE_SIZE * 0.5f, 0.f };

					m_pClone->SetPosition(vRespawnPos);
					CreateObject(m_pClone);
					CCollider* pcol = m_pClone->GetCollider();
					CColliderRect* pRectCol = dynamic_cast<CColliderRect*>(pcol);
					Vector3 vv = pRectCol->GetScale();
					++m_iCurCitizenCnt;
				}
			}
			if (m_iCurCitizenCnt > m_imaxCitizenCnt)
				break;
		}
		m_fCreateCoolTime = 0.f;
	}

	// 주기적으로 해제
	m_fDeleteCoolTime += DeltaTime;
	if (m_fDeleteCoolTime >= m_fMaxDeleteCoolTime) {
		int iLTCol;
		int iLTRow;
		int iRBCol;
		int iRBRow;
		GetEnableCreateArea(iLTCol, iLTRow, iRBCol, iRBRow);

		vector<CObject*>& vecCitizen = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::CITIZEN);

		auto iter = vecCitizen.begin();
		for (iter; iter != vecCitizen.end(); ++iter) {
			int iRow = int((*iter)->PosY() / TILE_SIZE);
			int iCol = int((*iter)->PosX() / TILE_SIZE);
			// 영역 밖이면 해제한다.
			if (iRow < iLTRow || iCol < iLTCol || iRow > iRBRow || iCol > iRBCol) {
				if ((*iter)->IsActive()) {
					DestroyObject((*iter));
					--m_iCurCitizenCnt;
				}
			}
		}
		m_fDeleteCoolTime = 0.f;
	}
}


void CGTA_CitizenCreator::Render(HDC _hDC)
{

}

void CGTA_CitizenCreator::GetEnableCreateArea(int& _iLTCol, int& _iLTRow, int& _iRBCol, int& _iRBRow)
{
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();

	// 카메라 바깥의 영역을 가져온다.
	POINT iResolution = CCore::GetInstance()->GetResolution();
	Vector3 vLeftTopWorldPos = m_pCamera->GetScreenToWorldPosition(Vector3(0.f, 0.f, 0.f));
	Vector3 vRightBottomWorldPos = m_pCamera->GetScreenToWorldPosition(Vector3((float)iResolution.x, (float)iResolution.y, 0));


	int iLTCol = int(vLeftTopWorldPos.x / TILE_SIZE);
	int iLTRow = int(vLeftTopWorldPos.y / TILE_SIZE);
	iLTCol -= m_iCreateMargin;
	iLTRow -= m_iCreateMargin;
	iLTCol = max(0, iLTCol);
	iLTRow = max(0, iLTRow);

	int iRBCol = (int)(vRightBottomWorldPos.x / TILE_SIZE);
	int iRBRow = (int)(vRightBottomWorldPos.y / TILE_SIZE);
	iRBCol += m_iCreateMargin;
	iRBRow += m_iCreateMargin;
	iRBCol = min((int)pTileMap->GetCol(), iRBCol);
	iRBRow = min((int)pTileMap->GetRow(), iRBRow);

	_iLTCol = iLTCol;
	_iLTRow = iLTRow;
	_iRBCol = iRBCol;
	_iRBRow = iRBRow;
}