#include "stdafx.h"7
#include "CScene_GtaInGame.h"

#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CKeyManager.h"
#include "CScene.h"
#include "CSceneManager.h"

#include "CTexture.h"

#include "CGTA_Vehicle.h"
#include "CCar.h"

#include "CCamera2D.h"
#include "CObject.h"
#include "CTileMap.h"


#include "CAnimator.h"
#include "CColliderRect.h"
#include "CGTA_PoliceCar.h"
#include "CGTA_Player.h"
#include "CGTA_CitizenCreator.h"

#include "CGTA_Item.h"

CScene_GtaInGame::CScene_GtaInGame()
{
}

CScene_GtaInGame::~CScene_GtaInGame()
{
}

void CScene_GtaInGame::Start()
{
	// Ÿ�ϸ� Load
	m_pTileMap = new CTileMap(E_GroupType::TILEMAP);
	LoadTile(STR_FILE_PATH_GTA_TILES_Save);
	AddObject(m_pTileMap);

	//// �÷��̾� �߰�
	CGTA_Player* pPlayer = new CGTA_Player(E_GroupType::PLAYER);
	pPlayer->Init();
	pPlayer->SetPosition(800, 800, 0);
	AddObject(pPlayer);


	////// Camera �߰�
	CCamera2D* pCamera = new CCamera2D(E_GroupType::MAIN_CAMERA);
	pCamera->Init();
	pCamera->SetMainCamera();
	pCamera->SetTargetObject(pPlayer);
	pCamera->SetLookAt(pPlayer->GetPosition());
	pCamera->AddEffect(E_CamEffect::FADE_IN, 3.f);
	AddObject((CObject*)pCamera);
	
	// �ڵ����� 
	// �ùε��� �ڵ������� �������� �Ŵ��� ������Ʈ�� �ϳ� �����ϰڽ��ϴ�.
	
	// Vehicle �Ŵ��� �߰�
	// CVehicleManager;

	// Citizen �Ŵ��� �߰�
	CGTA_CitizenCreator* pCitizenCreator = new CGTA_CitizenCreator(E_GroupType::DEFAULT);
	pCitizenCreator->Init();
	pCitizenCreator->SetCamera(pCamera);
	AddObject((CObject*)pCitizenCreator);
	

	// TODO : Item ����, Item �Ŵ��� ����
	// Item ����.
	// item ���� save, load �����
	LoadItem(STR_FILE_PATH_GTA_Item_Save);
	

	// ���� �ý���
	
	// UI �߰�

	// �浹���� ����
	CCollisionManager::GetInstance()->ClearAllCollisionGroup();
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::CITIZEN, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::VEHICLE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::PROJECTILE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::TILE, true);

	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::CITIZEN, E_GroupType::VEHICLE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::CITIZEN, E_GroupType::PROJECTILE, true);

	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::VEHICLE, E_GroupType::PROJECTILE, true);
}

void CScene_GtaInGame::PrevUpdate()
{
	CScene::PrevUpdate();
}

void CScene_GtaInGame::Update()
{
	CScene::Update();

	if (InputKeyPress(E_Key::F4))
		CSceneManager::GetInstance()->ChangeScene(E_SceneType::TOOL);
}

void CScene_GtaInGame::LateUpdate()
{
	CScene::LateUpdate();
}

void CScene_GtaInGame::End()
{
	DeleteAllObjects();
}

void CScene_GtaInGame::Render(HDC _hDC)
{
	CScene::Render(_hDC);
}