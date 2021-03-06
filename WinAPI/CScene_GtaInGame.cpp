#include "stdafx.h"
#include "CScene_GtaInGame.h"

#include "CResourceManager.h"
#include "CResource.h"
#include "CCollisionManager.h"
#include "CKeyManager.h"
#include "CScene.h"
#include "CSceneManager.h"

#include "CTexture.h"
#include "CSound.h"

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
#include "CGTA_PlayerUI.h"
#include "CGTA_SuspectSearchSystem.h"
#include "CDebug.h"
CScene_GtaInGame::CScene_GtaInGame()
{
}

CScene_GtaInGame::~CScene_GtaInGame()
{
}

void CScene_GtaInGame::Start()
{
	// Sound 로드 테스트
	CSound* pNewSound = CResourceManager::GetInstance()->GetSound(STR_FILE_PATH_GTA_Sound_OutdoorNoise_Sound, STR_FILE_PATH_GTA_Sound_OutdoorNoise_Sound);
	pNewSound->SetPosition(0.f); // 백분률, 소리 위치 설정
	pNewSound->PlayToBGM(true);
	pNewSound->SetVolume(60.f);

	// 타일맵 Load
	m_pTileMap = new CTileMap(E_GroupType::TILEMAP);
	LoadTile(STR_FILE_PATH_GTA_TILES_Save);
	AddObject(m_pTileMap);

	//// 플레이어 추가
	/*CGTA_Player* pPlayer = new CGTA_Player(E_GroupType::PLAYER);
	pPlayer->Init();
	pPlayer->SetPosition(800, 800, 0);
	AddObject(pPlayer);*/

	// UI Container를 하나 만들어서
	// 모든 UI들을 관리하게 한다.
	CGTA_UIContainer* pUIContainer = new CGTA_UIContainer(E_GroupType::UI_CONTAINER);
	pUIContainer->Init();
	AddObject(pUIContainer);
	LoadAll();

	/*CGTA_Citizen* pCitizen = new CGTA_Citizen(E_GroupType::CITIZEN);
	pCitizen->Init();
	pCitizen->SetPosition(600, 600);
	AddObject(pCitizen);

	CGTA_Cop* pCop = new CGTA_Cop(E_GroupType::CITIZEN);
	pCop->Init();
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

	for (int i = 0; i < 10; ++i) {
		AddObject(pCop->Clone());
	}*/
	
	// 자동차나 
	// 시민들을 자동생성을 관리해줄 매니저 오브젝트를 하나 생성하겠습니다.
	CGTA_PoliceCar* pPoliceCar = new CGTA_PoliceCar(E_GroupType::VEHICLE);
	pPoliceCar->Init();
	pPoliceCar->SetPosition(1200, 1200);
	AddObject(pPoliceCar);

	CGTA_PoliceCar* pPol2 = pPoliceCar->Clone();
	pPol2->SetPosition(1200, 1500);
	AddObject(pPol2);

	CGTA_PoliceCar* pPol3 = pPoliceCar->Clone();
	pPol3->SetPosition(889.20f, 5836.67f);
	pPol3->SetTexture(CResourceManager::GetInstance()->GetTexture(STR_FILE_PATH_Vehicle_car_Normal, STR_FILE_PATH_Vehicle_car_Normal));
	CSound* pEngineSound = CResourceManager::GetInstance()->GetSound(Sound_Car_Sport_engine, Sound_Car_Sport_engine);
	pPol3->SetEngineSound(pEngineSound);
	pPol3->VehicleInfo().fPower = pPol3->VehicleInfo().fPower + 10.f;
	AddObject(pPol3);

	CGTA_PoliceCar* pPol4 = pPol3->Clone();
	pPol4->SetPosition(5779.33f, 3568.92f);
	pPol4->RotateRP(-90.f);
	AddObject(pPol4);


	// Vehicle 매니저 추가
	// CVehicleManager;

	// 수배 시스템
	CGTA_SuspectSearchSystem* pSusSearchSystem = new CGTA_SuspectSearchSystem(E_GroupType::MANAGER);
	pSusSearchSystem->Init();
	AddObject(pSusSearchSystem);

	// TODO : Item 구현, Item 매니저 구현
	// Item 설정.
	// item 정보 save, load 만들기

	
	////// Camera 추가
	CGTA_Player* pPlayer = (CGTA_Player*)CSceneManager::GetInstance()->GetCurScene()->FindObject(STR_OBJECT_NAME_Player);
	CCamera2D* pCamera = new CCamera2D(E_GroupType::MAIN_CAMERA);
	pCamera->Init();
	pCamera->SetMainCamera();
	pCamera->SetTargetObject(pPlayer);
	pCamera->SetLookAt(pPlayer->GetPosition());
	pCamera->AddEffect(E_CamEffect::FADE_IN, 3.f);
	AddObject((CObject*)pCamera);


	// Citizen 매니저 추가
	CGTA_CitizenCreator* pCitizenCreator = new CGTA_CitizenCreator(E_GroupType::DEFAULT);
	pCitizenCreator->Init();
	pCitizenCreator->SetCamera(pCamera);
	AddObject((CObject*)pCitizenCreator);

	// 충돌영역 설정
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
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::VEHICLE, E_GroupType::VEHICLE, true);

	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::TILE, E_GroupType::PROJECTILE, true);
}

void CScene_GtaInGame::PrevUpdate()
{
	CScene::PrevUpdate();
}

void CScene_GtaInGame::Update()
{
	Debug->Print(Vector3(MousePosition.x, MousePosition.y), L"dd", MainCamera->GetScreenToWorldPosition(MousePosition).x, MainCamera->GetScreenToWorldPosition(MousePosition).y);

	CScene::Update();

	if (InputKeyPress(E_Key::F4))
		CSceneManager::GetInstance()->ChangeScene(E_SceneType::TOOL);

	if (Debug->IsShow())
		CCollider::SetRenderActive(true);
	else
		CCollider::SetRenderActive(false);
}

void CScene_GtaInGame::LateUpdate()
{
	CScene::LateUpdate();
}

void CScene_GtaInGame::End()
{
	DeleteAllObjects();
	CResourceManager::GetInstance()->InitAllSounds();
}

void CScene_GtaInGame::Render(HDC _hDC)
{
	CScene::Render(_hDC);
}