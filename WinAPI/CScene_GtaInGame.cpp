#include "stdafx.h"
#include "CScene_GtaInGame.h"

#include "CResourceManager.h"

#include "CTexture.h"

#include "CVehicle.h"
#include "CCar.h"


#include "CAnimator.h"
#include "CColliderRect.h"

CScene_GtaInGame::CScene_GtaInGame()
{
}

CScene_GtaInGame::~CScene_GtaInGame()
{
}

void CScene_GtaInGame::Start()
{
	// Texture Setting
	CTexture* pVehicleTex = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_Vehicle_car_Cops1);
	if (nullptr ==pVehicleTex)
		pVehicleTex = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_Vehicle_car_Cops1,STR_FILE_PATH_Vehicle_car_Cops1);

	CTexture* pVehicleTexAnim = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_Vehicle_car_Cops1Anim);
	if (nullptr == pVehicleTexAnim)
		pVehicleTexAnim = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_Vehicle_car_Cops1Anim, STR_FILE_PATH_Vehicle_car_Cops1Anim);

	CVehicle* pVehicle = new CCar(E_GroupType::VEHICLE);
	pVehicle->SetTexture(pVehicleTexAnim);
	CAnimator* pAnimator = new CAnimator(pVehicle);
	float fSizeX = pVehicleTexAnim->GetWidth() / 10.0f;
	float fSizeY = (pVehicleTexAnim->GetHeight()) / 2.0f;
	float gap = 4.0f;

	pAnimator->CreateAnimation(L"Normal", pVehicleTexAnim, Vector2(0.f, fSizeY), Vector2((float)pVehicleTex->GetWidth(), (float)pVehicleTex->GetHeight()), Vector2(4, 4), 1, 1);
	

	pAnimator->PlayAnimation(L"Normal", E_AnimationPlayType::LOOP);
	pVehicle->SetPosition(100, 100, 0);
	pVehicle->SetScale(100, 100);

	

	AddObject(pVehicle);
}


void CScene_GtaInGame::PrevUpdate()
{
	CScene::PrevUpdate();
}


void CScene_GtaInGame::Update()
{
	CScene::Update();
}

void CScene_GtaInGame::LateUpdate()
{
	CScene::LateUpdate();
}

void CScene_GtaInGame::End()
{
}

void CScene_GtaInGame::Render(HDC _hDC)
{
	CScene::Render(_hDC);
}