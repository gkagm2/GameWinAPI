#include "stdafx.h"
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
#include "CGTA_Citizen.h"
#include "CGTA_Cop.h"

#include "CGTA_Item.h"
#include "CGTA_Bullet.h"

#include "CGTA_UIContainer.h"
#include "CGTA_WeaponUI.h"

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
	/*CGTA_Player* pPlayer = new CGTA_Player(E_GroupType::PLAYER);
	pPlayer->Init();
	pPlayer->SetPosition(800, 800, 0);
	AddObject(pPlayer);*/


	// UI Container�� �ϳ� ����
	// ��� UI���� �����ϰ� �Ѵ�.
	CGTA_UIContainer* pUIContainer = new CGTA_UIContainer(E_GroupType::UI_CONTAINER);
	pUIContainer->Init();
	AddObject(pUIContainer);

	LoadAll();


	CGTA_Citizen* pCitizen = new CGTA_Citizen(E_GroupType::CITIZEN);
	pCitizen->Init();
	pCitizen->SetPosition(600, 600);
	AddObject(pCitizen);

	CGTA_Cop* pCop = new CGTA_Cop(E_GroupType::CITIZEN);
	pCop->Init();
	E_WeaponType eWeap = pCop->GetCurWeaponType();
	pCop->SetPosition(800, 600);
	AddObject(pCop);

	CGTA_Cop* pCop1 = new CGTA_Cop(E_GroupType::CITIZEN);
	pCop1->Init();
	TWeaponInfo tWeaponInfo;
	tWeaponInfo.InitWeapon(E_WeaponType::PISTOL);
	tWeaponInfo.bIsInfinite = true;
	pCop1->SetWeaponInfo(E_WeaponType::PISTOL, tWeaponInfo);
	pCop1->SetWeaponState(true, E_WeaponType::PISTOL);
	pCop1->SelectWeapon(E_WeaponType::PISTOL);
	E_WeaponType e = pCop1->GetCurWeaponType();
	pCop1->SetPosition(800, 700);
	AddObject(pCop1);

	
	// �ڵ����� 
	// �ùε��� �ڵ������� �������� �Ŵ��� ������Ʈ�� �ϳ� �����ϰڽ��ϴ�.
	CGTA_PoliceCar* pPoliceCar = new CGTA_PoliceCar(E_GroupType::VEHICLE);
	pPoliceCar->Init();
	pPoliceCar->SetPosition(1200, 1200);
	AddObject(pPoliceCar);

	// Vehicle �Ŵ��� �߰�
	// CVehicleManager;



	// Citizen �Ŵ��� �߰�
	/*CGTA_CitizenCreator* pCitizenCreator = new CGTA_CitizenCreator(E_GroupType::DEFAULT);
	pCitizenCreator->Init();
	pCitizenCreator->SetCamera(pCamera);
	AddObject((CObject*)pCitizenCreator);*/


	// TODO : Item ����, Item �Ŵ��� ����
	// Item ����.
	// item ���� save, load �����

	// ���� �ý���
	
	////// Camera �߰�
	CGTA_Player* pPlayer = (CGTA_Player*)CSceneManager::GetInstance()->GetCurScene()->FindObject(L"Player");
	CCamera2D* pCamera = new CCamera2D(E_GroupType::MAIN_CAMERA);
	pCamera->Init();
	pCamera->SetMainCamera();
	pCamera->SetTargetObject(pPlayer);
	pCamera->SetLookAt(pPlayer->GetPosition());
	pCamera->AddEffect(E_CamEffect::FADE_IN, 3.f);
	AddObject((CObject*)pCamera);

	// Render X
	//CCollider::SetRenderActive(false);

	// �浹���� ����
	CCollisionManager::GetInstance()->ClearAllCollisionGroup();
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::CITIZEN, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::VEHICLE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::PROJECTILE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::TILE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::ITEM, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::PUNCH, true);

	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::CITIZEN, E_GroupType::VEHICLE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::CITIZEN, E_GroupType::PROJECTILE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::CITIZEN, E_GroupType::PUNCH, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::CITIZEN, E_GroupType::CITIZEN, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::CITIZEN, E_GroupType::TILE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::CITIZEN, E_GroupType::PUNCH, true);

	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::VEHICLE, E_GroupType::PROJECTILE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::VEHICLE, E_GroupType::TILE, true);

	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::TILE, E_GroupType::PROJECTILE, true);
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