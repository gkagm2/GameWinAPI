#include "stdafx.h"
#include "CGTA_CitizenCreator.h"
#include "CGTA_Citizen.h"
#include "CGTA_Cop.h"
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
	m_pCamera(nullptr),
	m_iCurCitizenCnt(0),
	m_imaxCitizenCnt(80),
	m_fCreateCoolTime(7.f),
	m_fMaxCreateCoolTime(7.f),
	m_fDeleteCoolTime(0.f),
	m_fMaxDeleteCoolTime(3.f),
	m_fDeleteDistance(1000.f),
	m_fCreatableMinDistance(0.f),
	m_fCreatableMaxDistance(0.f),
	m_pPlayer(nullptr)
{
	Vector2 vResolution = CCore::GetInstance()->GetResolution();
	/*float fDistance = vResolution.GetDistance();
	m_fDeleteDistance = fDistance + fDistance * 0.5f;

	m_fCreatableMinDistance = fDistance * 0.7f;
	m_fCreatableMaxDistance = m_fDeleteDistance * 0.95f;*/


	float fDistance = vResolution.GetDistance();
	m_fDeleteDistance = fDistance;

	m_fCreatableMinDistance = fDistance * 0.5f;
	m_fCreatableMaxDistance = m_fDeleteDistance * 0.95f;
}

CGTA_CitizenCreator::~CGTA_CitizenCreator()
{
}

void CGTA_CitizenCreator::Init()
{
	if (nullptr == m_pPlayer) {
		m_pPlayer = CSceneManager::GetInstance()->GetCurScene()->FindObject(STR_OBJECT_NAME_Player);
		assert(m_pPlayer);
	}
}

void CGTA_CitizenCreator::Update()
{
	if (nullptr == m_pPlayer) {
		m_pPlayer = CSceneManager::GetInstance()->GetCurScene()->FindObject(STR_OBJECT_NAME_Player);
		assert(m_pPlayer);
		return;
	}

	
	vector<CObject*> vecObjs = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::CITIZEN);
	m_iCurCitizenCnt = (int)vecObjs.size();

	// ���� �Ÿ� �̻� ������ �ù� ����
	for (UINT i = 0; i < vecObjs.size(); ++i) {
		float fDistance = CMyMath::GetDistance(vecObjs[i]->GetPosition(), m_pPlayer->GetPosition());
		if (fDistance > m_fDeleteDistance) {
			DestroyObject(vecObjs[i]);
		}
	}
	// ���� �Ÿ� �̻� ������ ���� ����
	vector<CObject*> vecVehicleObjs = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::VEHICLE);
	for (UINT i = 0; i < vecVehicleObjs.size(); ++i) {
		float fDistance = CMyMath::GetDistance(vecVehicleObjs[i]->GetPosition(), m_pPlayer->GetPosition());
		if (fDistance > m_fDeleteDistance) {
			DestroyObject(vecVehicleObjs[i]);
		}
	}

	// �����Ÿ� ������ �����ԵǸ� �ù� ����.
	// �ֱ������� ����
	m_fCreateCoolTime += DeltaTime;
	if (m_fCreateCoolTime >= m_fMaxCreateCoolTime) {
		// ���� �ִ� ��� Ÿ�ϵ��� �ҷ���
		vector<CObject*>& vecTiles = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);

		// Ÿ�ϸ��� ������
		CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
		if (nullptr == pTileMap) {
			assert(pTileMap);
			return;
		}
		vector<CObject*>& vecCitizen = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::CITIZEN);
		
		// �ù� ����
		for (int i = 0; i < 8; ++i) { // 2�� �õ��Ѵ�.
			Vector3 vRespawnPos;
			TTilePos tRespawnPos;
			int  tryCnt = 0;
			int iTileIdx = -1;
			while (tryCnt <= 20) {
				++tryCnt;
				float fRandomDegree = (float)(rand() % 360);
				Vector3 vRandomDir = Rotate(Vector3{ 0,-1,0 }, fRandomDegree); // �������� ������ ���� ����

				// ������ �� �ִ� �Ÿ��� ���Ѵ�.
				float fCreatableDistance = m_fCreatableMinDistance + rand() % (int)(m_fCreatableMaxDistance - m_fCreatableMinDistance);

				// ���� �� ������
				vRespawnPos = m_pPlayer->GetPosition() + vRandomDir * fCreatableDistance;
				int iMaxCol = (int)pTileMap->GetCol() * TILE_SIZE;
				int iMaxRow = (int)pTileMap->GetRow() * TILE_SIZE;
				if (0 > vRespawnPos.x || 0 > vRespawnPos.y || vRespawnPos.x > iMaxCol || vRespawnPos.y > iMaxRow) {
					continue;
				}
				tRespawnPos = pTileMap->VectorToTilePos(vRespawnPos); // Ÿ�� ��ġ������ ��ȯ

				// ������ ����� �ٽ��Ѵ�.
				iTileIdx = tRespawnPos.y * pTileMap->GetCol() + tRespawnPos.x;
				if (iTileIdx > (int)pTileMap->GetRow() * (int)pTileMap->GetCol() - 1 ||
					iTileIdx < 0) {
					continue;
				}
				break;
			}
			if (iTileIdx < 0)
				continue;

			CTile* pTile = dynamic_cast<CTile*>(vecTiles[iTileIdx]);
			assert(pTile);

			if (E_TileType::Road == pTile->GetTileType() || E_TileType::Sidewalk == pTile->GetTileType()) {
				// �ù��� ����.
				CGTA_Citizen* m_pCitizen = new CGTA_Citizen(E_GroupType::CITIZEN);
				m_pCitizen->SetObjectName(L"Citizen");
				m_pCitizen->Init();
				m_pCitizen->SetPosition(pTileMap->TilePosToVector(tRespawnPos));
				CreateObject(m_pCitizen);
				++m_iCurCitizenCnt;
			}
		}

		// ���� ����
		for (int i = 0; i < 2; ++i) { // 10�� �õ��Ѵ�.
			Vector3 vRespawnPos;
			TTilePos tRespawnPos;
			int  tryCnt = 0;
			int iTileIdx = -1;
			while (tryCnt <= 20) {
				++tryCnt;
				float fRandomDegree = (float)(rand() % 360);
				Vector3 vRandomDir = Rotate(Vector3{ 0,-1,0 }, fRandomDegree); // �������� ������ ���� ����

				// ������ �� �ִ� �Ÿ��� ���Ѵ�.
				float fCreatableDistance = m_fCreatableMinDistance + rand() % (int)(m_fCreatableMaxDistance - m_fCreatableMinDistance);

				// ���� �� ������
				vRespawnPos = m_pPlayer->GetPosition() + vRandomDir * fCreatableDistance;
				int iMaxCol = (int)pTileMap->GetCol() * TILE_SIZE;
				int iMaxRow = (int)pTileMap->GetRow() * TILE_SIZE;
				if (0 > vRespawnPos.x || 0 > vRespawnPos.y || vRespawnPos.x > iMaxCol || vRespawnPos.y > iMaxRow) {
					continue;
				}
				tRespawnPos = pTileMap->VectorToTilePos(vRespawnPos); // Ÿ�� ��ġ������ ��ȯ

				// ������ ����� �ٽ��Ѵ�.
				iTileIdx = tRespawnPos.y * pTileMap->GetCol() + tRespawnPos.x;
				if (iTileIdx > (int)pTileMap->GetRow() * (int)pTileMap->GetCol() - 1 ||
					iTileIdx < 0) {
					continue;
				}
				break;
			}
			if (iTileIdx < 0)
				continue;

			CTile* pTile = dynamic_cast<CTile*>(vecTiles[iTileIdx]);

			if (E_TileType::Road == pTile->GetTileType() || E_TileType::Sidewalk == pTile->GetTileType()) {
				// ���� ����.
				CGTA_Cop* pCop1 = new CGTA_Cop(E_GroupType::CITIZEN);
				pCop1->Init();
				pCop1->SetActive(true);
				pCop1->SetPosition(pTileMap->TilePosToVector(tRespawnPos));
				CreateObject(pCop1);
				++m_iCurCitizenCnt;
			}
		}

		// �ѵ� ���� ����
		for (int i = 0; i < 1; ++i) { // 10�� �õ��Ѵ�.
			Vector3 vRespawnPos;
			int  tryCnt = 0;
			int iTileIdx = -1;
			while (tryCnt <= 20) {
				++tryCnt;
				float fRandomDegree = (float)(rand() % 360);
				Vector3 vRandomDir = Rotate(Vector3{ 0,-1,0 }, fRandomDegree); // �������� ������ ���� ����

				// ������ �� �ִ� �Ÿ��� ���Ѵ�.
				float fCreatableDistance = m_fCreatableMinDistance + rand() % (int)(m_fCreatableMaxDistance - m_fCreatableMinDistance);

				// ���� �� ������
				vRespawnPos = m_pPlayer->GetPosition() + vRandomDir * fCreatableDistance;
				int iMaxCol = (int)pTileMap->GetCol() * TILE_SIZE;
				int iMaxRow = (int)pTileMap->GetRow() * TILE_SIZE;
				if (0 > vRespawnPos.x || 0 > vRespawnPos.y || vRespawnPos.x > iMaxCol || vRespawnPos.y > iMaxRow) {
					continue;
				}
				TTilePos tRespawnPos = pTileMap->VectorToTilePos(vRespawnPos); // Ÿ�� ��ġ������ ��ȯ

				// ������ ����� �ٽ��Ѵ�.
				iTileIdx = tRespawnPos.y * pTileMap->GetCol() + tRespawnPos.x;
				if (iTileIdx > (int)pTileMap->GetRow() * (int)pTileMap->GetCol() - 1 ||
					iTileIdx < 0) {
					continue;
				}
				break;
			}
			if (iTileIdx < 0)
				continue;

			CTile* pTile = dynamic_cast<CTile*>(vecTiles[iTileIdx]);
			assert(pTile);

			if (E_TileType::Road == pTile->GetTileType() || E_TileType::Sidewalk == pTile->GetTileType()) {
				// ���� ����.
				CGTA_Cop* pCop1 = new CGTA_Cop(E_GroupType::CITIZEN);
				pCop1->Init();
				TWeaponInfo tWeaponInfo;
				tWeaponInfo.InitWeapon(E_WeaponType::PISTOL);
				tWeaponInfo.bIsInfinite = true;
				pCop1->SetPosition(vRespawnPos);
				pCop1->SetWeaponInfo(E_WeaponType::PISTOL, tWeaponInfo);
				pCop1->SetWeaponState(true, E_WeaponType::PISTOL);
				pCop1->SelectWeapon(E_WeaponType::PISTOL);
				CreateObject(pCop1);
				++m_iCurCitizenCnt;
			}
		}
		m_fCreateCoolTime = 0.f;
	}
	Debug->Print(Vector2(200, 50), L"i", m_iCurCitizenCnt);

}
void CGTA_CitizenCreator::Render(HDC _hDC)
{
}

// TODO : ���� ���� �Լ�
void CGTA_CitizenCreator::GetEnableCreateArea(int& _iLTCol, int& _iLTRow, int& _iRBCol, int& _iRBRow)
{
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();

	// ī�޶� �ٱ��� ������ �����´�.
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