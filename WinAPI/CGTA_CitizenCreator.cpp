#include "CGTA_CitizenCreator.h"
#include "CGTA_Citizen.h"
#include "CCamera2D.h"
#include "CCore.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CTile.h"
#include "CTileMap.h"
#include "CKeyManager.h"

CGTA_CitizenCreator::CGTA_CitizenCreator(E_GroupType e_GroupType) :
	CObject(e_GroupType),
	m_iCreateMargin(3),
	m_pCitizenObjPrefab(nullptr),
	m_pCamera(nullptr)
{
	if (m_pCitizenObjPrefab)
		delete m_pCitizenObjPrefab;
}

CGTA_CitizenCreator::~CGTA_CitizenCreator()
{
}

void CGTA_CitizenCreator::Init()
{
	m_pCitizenObjPrefab = new  CGTA_Citizen(E_GroupType::CITIZEN);
}

void CGTA_CitizenCreator::Update()
{
	if (InputKeyPress(E_Key::Q)) {
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
		Vector3 vLeftTopWorldPos = m_pCamera->GetScreenToWorldPosition(Vector3(0.f,0.f,0.f));
		Vector3 vRightBottomWorldPos = m_pCamera->GetScreenToWorldPosition(Vector3(iResolution.x, iResolution.y, 0));

		int iLTCol = vLeftTopWorldPos.x / TILE_SIZE;
		int iLTRow = vLeftTopWorldPos.y / TILE_SIZE;
		iLTCol -= m_iCreateMargin;
		iLTRow -= m_iCreateMargin;
		iLTCol = max(0, iLTCol);
		iLTRow = max(0, iLTRow);

		int iRBCol = vRightBottomWorldPos.x / TILE_SIZE;
		int iRBRow = vRightBottomWorldPos.y / TILE_SIZE;
		iRBCol += m_iCreateMargin;
		iRBRow += m_iCreateMargin;
		iRBCol = min(pTileMap->GetCol(), iRBCol);
		iRBRow = min(pTileMap->GetRow(), iRBRow);

		for (int iRow = iLTRow; iRow <= iRBRow; ++iRow) {
			for (int iCol = iLTCol; iCol <= iRBCol; ++iCol) {
				CTile* pTile = dynamic_cast<CTile*>(vecTiles[pTileMap->GetCol() * iRow + iCol]);
				if (nullptr == pTile)
					continue;

				if (E_TileType::Road == pTile->GetTileType() || E_TileType::Sidewalk == pTile->GetTileType()) {
					// 시민을 생성.
					CGTA_Citizen* m_pClone = m_pCitizenObjPrefab->Clone();
					CreateObject(m_pClone);

					// 시민 위치 설정.
					Vector3 vRespawnPos = { iCol * TILE_SIZE + TILE_SIZE * 0.5f, iRow * TILE_SIZE + TILE_SIZE * 0.5f, 0.f };
					m_pClone->SetPosition(vRespawnPos);
				}
			}
		}
	}
}