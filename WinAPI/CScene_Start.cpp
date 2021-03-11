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

#include "CAnimator.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Start()
{
	POINT ptResolution = CCore::GetInstance()->GetResolution();

	// 백그라운드 관리자 생성
	//CBackgroundController* pBackgroundCtr = new CBackgroundController(E_GroupType::BACKGROUND);
	//AddObject(pBackgroundCtr);

	// 플레이어 텍스쳐 로딩
	CTexture* pPlayerTexture2 = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_PlayerAnim);
	if (nullptr == pPlayerTexture2)
		pPlayerTexture2 = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_PlayerAnim, STR_FILE_PATH_PlayerAnim);
	
	// 플레이어 오브젝트 생성
	CPlayer* pPlayer = new CPlayer(E_GroupType::PLAYER);
	pPlayer->SetObjectName(L"Player");
	pPlayer->SetPosition(Vector3{ ptResolution.x / 2.0f, ptResolution.y - 100.0f, .0f });
	pPlayer->SetTexture(pPlayerTexture2); // 텍스쳐 설정
	Vector2 vScale =ResizeScaleMaintainRatio(50, 50, pPlayer->GetTextureHeight(), pPlayer->GetTextureWidth());
	pPlayer->SetScale(Vector3(vScale.x, vScale.y, 1.0f));


	// 플레이어 충돌 컴포넌트 생성 및 추가
	CColliderRect* pPlayerRectCollider = new CColliderRect(pPlayer);
	pPlayerRectCollider->SetScale(Vector3(20.0f, 30.0f, 0.0f));
	pPlayerRectCollider->SetOffsetPosition(Vector3(0.0f, 10.0f, 0.0f));
	AddObject(pPlayer);

	// 플레이어 애니메이터 생성 및 추가
	CAnimator* pPlayerAnimator = new CAnimator(pPlayer);

	// 애니메이션 추가
	/*pPlayerAnimator->CreateAnimation(L"IDLE_DOWN", pPlayerTexture2, Vector2(0, 65 * 0), Vector2(60, 65), 3, 0.1f);
	pPlayerAnimator->CreateAnimation(L"IDLE_LEFT", pPlayerTexture2, Vector2(0, 65 * 1), Vector2(60, 65), 3, 0.1f);
	pPlayerAnimator->CreateAnimation(L"IDLE_UP", pPlayerTexture2, Vector2(0, 65 * 2), Vector2(60, 65), 1, 0.1f);
	pPlayerAnimator->CreateAnimation(L"IDLE_RIGHT", pPlayerTexture2, Vector2(0, 65 * 3), Vector2(60, 65), 3, 0.1f);
	pPlayerAnimator->CreateAnimation(L"WALK_DOWN", pPlayerTexture2, Vector2(0, 65 * 4), Vector2(60, 65), 10, 0.1f);
	pPlayerAnimator->CreateAnimation(L"WALK_LEFT", pPlayerTexture2, Vector2(0, 65 * 5), Vector2(60, 65), 10, 0.1f);
	pPlayerAnimator->CreateAnimation(L"WALK_UP", pPlayerTexture2, Vector2(0, 65 * 6), Vector2(60, 65), 10, 0.1f);
	pPlayerAnimator->CreateAnimation(L"WALK_RIGHT", pPlayerTexture2, Vector2(0, 65 * 7), Vector2(60, 65), 10, 0.1f);

	pPlayerAnimator->PlayAnimation(L"IDLE_DOWN", E_AnimationPlayType::LOOP);
	pPlayerAnimator->Save(STR_FILE_PATH_PlayerAnim_Save);*/

	pPlayerAnimator->Load(STR_FILE_PATH_PlayerAnim_Save);
	pPlayerAnimator->PlayAnimation(L"WALK_UP", E_AnimationPlayType::LOOP);

	//// 몬스터 텍스쳐 로딩
	//CTexture * pMonsterTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_Monster, STR_FILE_PATH_Monster);

	//// Monster Respawner 생성
	//CMonsterRespawner* pMonsterRespawner = new CMonsterRespawner(E_GroupType::MONSTER);
	//pMonsterRespawner->SetRender(false);
	//pMonsterRespawner->SetObjectName(L"Monster Respawner");
	//AddObject(pMonsterRespawner);
	
	// 충돌 설정
	CCollisionManager::GetInstance()->ClearAllCollisionGroup();
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::MONSTER, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER, E_GroupType::MONSTER_PROJECTILE, true);
	CCollisionManager::GetInstance()->SetOnOffCollisionGroup(E_GroupType::PLAYER_PROJECTILE, E_GroupType::MONSTER, true);

	// 카메라 Look 설정
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