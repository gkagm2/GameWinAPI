#include "stdafx.h"
#include "CScene_GtaInGame.h"

#include "CResourceManager.h"

#include "CTexture.h"

#include "CVehicle.h"
#include "CCar.h"


#include "CAnimator.h"
#include "CColliderRect.h"
#include "CGTA_PoliceCar.h"
#include "CGTA_Player.h"

CScene_GtaInGame::CScene_GtaInGame()
{
}

CScene_GtaInGame::~CScene_GtaInGame()
{
}

void CScene_GtaInGame::Start()
{
	CGTA_PoliceCar* pVehicle = new CGTA_PoliceCar(E_GroupType::VEHICLE);
	pVehicle->SetObjectName(L"Police Car");
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