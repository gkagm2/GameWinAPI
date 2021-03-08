#include "stdafx.h"
#include "CScene_Start.h"
#include "CCore.h"
#include "CObject.h"

#include "CPathManager.h"
#include "CResourceManager.h"

#include "CCamera.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CMonsterRespawner.h"
#include "CBackgroundController.h"
#include "CBackground.h"

#include "CCollisionManager.h"
#include "CColliderCircle.h"
#include "CColliderRect.h"


CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Start()
{
	POINT ptResolution = CCore::GetInstance()->GetResolution();

	// ��׶��� �ؽ��� �ε�
	CBackgroundController* pBackgroundCtr = new CBackgroundController(E_GroupType::BACKGROUND);
	AddObject(pBackgroundCtr);

	// �÷��̾� �ؽ��� �ε�
	CTexture* pPlayerTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_Player, STR_FILE_PATH_Player);

	// ���� �ؽ��� �ε�
	CTexture* pMonsterTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_Monster, STR_FILE_PATH_Monster);


	// �÷��̾� ������Ʈ ����
	CPlayer* pPlayer = new CPlayer(E_GroupType::PLAYER);
	pPlayer->SetObjectName(L"Player");
	pPlayer->SetTexture(pPlayerTexture);
	pPlayer->SetPosition(Vector3{ ptResolution.x / 2.0f, ptResolution.y - 100.0f, .0f });
	pPlayer->SetScale(Vector3{ 80.0f, 80.0f ,1.0f });

	// �÷��̾� �浹 ������Ʈ ���� �� �߰�
	CColliderRect* pPlayerRectCollider = new CColliderRect(pPlayer);
	pPlayerRectCollider->SetScale(Vector3(20, 40, 0));
	pPlayerRectCollider->SetOffsetPosition(Vector3(0, 10, 0));
	AddObject(pPlayer);

	// Monster Respawner ����
	CMonsterRespawner* pMonsterRespawner = new CMonsterRespawner(E_GroupType::MONSTER);
	pMonsterRespawner->SetRender(false);
	pMonsterRespawner->SetObjectName(L"Monster Respawner");
	AddObject(pMonsterRespawner);
	


	// �浹 ����
	CCollisionManager::GetInstance()->ClearAllCollisionGroup();
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::MONSTER, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::MONSTER_PROJECTILE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER_PROJECTILE, E_GroupType::MONSTER, true);

	// ī�޶� Look ����
	Vector3 vResolution = ptResolution;
	vResolution /= 2.0f;
	CCamera::GetInstance()->SetLookAt(vResolution);

	HDC hDC = CCore::GetInstance()->GetDC();
	TextOut(hDC, 20, 20, L"Start Scene", (int)wcslen(L"Start Scene"));
}

void CScene_Start::End()
{
	DeleteAllObjects();
}