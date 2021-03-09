#include "stdafx.h"
#include "CMonsterRespawner.h"
#include "CMonster.h"
#include "CCore.h"
#include "CMonster.h"
#include "CMonster_Small.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CColliderRect.h"
#include "CColliderCircle.h"
#include "CTimeManager.h"

CMonsterRespawner::CMonsterRespawner(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_iPhase(0),
	m_vecSmallMonster{},
	respawnMaxCoolTime(1.0f),
	respawnCoolTime(1.0f),
	respawnCnt(0)
{
	InitRespawnMonster();
}

CMonsterRespawner::~CMonsterRespawner()
{
	m_vecSmallMonster.clear();
}

void CMonsterRespawner::Update()
{
	if (m_iPhase == 0) {
		// Random으로 위치를 변경하여 몬스터를 생성하게 하기
		if (IsAllDeadSmallMonster()) {
			m_vecSmallMonster.clear();
			++m_iPhase;
			//InitRespawnMonster();
		}
	}
	else  if (m_iPhase == 1) { // Create
		respawnCoolTime += DeltaTime;
		if (respawnCoolTime >= respawnMaxCoolTime) {
			if (respawnCnt >= 3) {
				++m_iPhase;
			}
			else {
				CreateMonster();
				respawnCoolTime = 0;
				++respawnCnt;
			}
		}
	}
	else if (m_iPhase == 2) {
		if (IsAllDeadMonster()) {
			++m_iPhase;
		}
	}
	else if (m_iPhase == 3) {
		InitRespawnMonster();
		CreateMonster();
		++m_iPhase;
	}
	else if (m_iPhase == 4) {
		if (IsAllDeadSmallMonster() && IsAllDeadMonster())
			++m_iPhase;
	}
	else if (m_iPhase >= 5) {
		DestroyObject(this);
	}
}

void CMonsterRespawner::LateUpdate()
{
}

void CMonsterRespawner::InitRespawnMonster() {
	for (int i = 0; i < 4 * 8; ++i)
		CreateSmallMonster();

	HDC hdc = CCore::GetInstance()->GetDC();
	Vector2 resolutionPos = CCore::GetInstance()->GetResolution();
	float yGap = 50.0f;
	for (int y = 0; y < 4; ++y) {
		float xGap = resolutionPos.x / (8.0f + 2.0f);
		for (int x = 0; x < 8; ++x) {
			Vector3 startPos(xGap, yGap, 0);
			m_vecSmallMonster[y * 8 + x]->SetPosition(startPos);
			m_vecSmallMonster[y * 8 + x]->SetStartPosition(startPos);
			xGap += resolutionPos.x / (8.0f + 2.0f);
		}
		yGap += 100.0f;
	}
}

bool CMonsterRespawner::IsAllDeadSmallMonster()
{
	vector<CMonster_Small*>::iterator iter = m_vecSmallMonster.begin();
	bool isZero = true;
	while (iter != m_vecSmallMonster.end()) {
		if ((*iter)->IsDead())
			iter = m_vecSmallMonster.erase(iter);
		else {
			isZero = false;
			++iter;
		}
	}
	return isZero;
}

bool CMonsterRespawner::IsAllDeadMonster()
{
	vector<CMonster*>::iterator iter = m_vecMonster.begin();
	bool isZero = true;
	while (iter != m_vecMonster.end()) {
		if ((*iter)->IsDead())
			iter = m_vecMonster.erase(iter);
		else {
			isZero = false;
			++iter;
		}
	}
	return isZero;
}

void CMonsterRespawner::CreateSmallMonster()
{
	// 몬스터 텍스쳐 로딩
	CTexture* pMonsterTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_Monster);

	CMonster_Small* pMonster = new CMonster_Small(E_GroupType::MONSTER);
	pMonster->SetObjectName(L"Monster");
	pMonster->SetTexture(pMonsterTexture);

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
	CObject* pPlayerObj = pCurScene->FindObject(L"Player");
	assert(nullptr != pPlayerObj);
	pMonster->SetTargetObj(pPlayerObj);
	pMonster->SetStartPosition(pMonster->GetPosition());
	pMonster->SetRange(150.0f);
	pMonster->SetSpeed(2.0f);
	pMonster->SetScale(Vector3(70.0f,70.0f,0));
	pMonster->SetOriginalScale(Vector3(70.0f, 70.0f, 0));
	
	// 몬스터 충돌 컴포넌트 생성 및 추가
	CColliderRect* pMonsterRectCollider = new CColliderRect(pMonster);
	pMonsterRectCollider->SetScale(pMonsterRectCollider->GetScale() - 20);

	m_vecSmallMonster.push_back(pMonster);
	CreateObject(pMonster);
}

void CMonsterRespawner::CreateMonster()
{
	CTexture* pMonsterTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_Monster);
	Vector3 ptResolution = CCore::GetInstance()->GetResolution();

	// 몬스터 오브젝트 생성
	CMonster* pMonster = new CMonster(E_GroupType::MONSTER);
	pMonster->SetObjectName(L"Monster2");
	pMonster->SetTexture(pMonsterTexture);
	pMonster->SetPosition(Vector2(ptResolution.x / 2.0f, 100));

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
	CObject* pPlayerObj = pCurScene->FindObject(L"Player");
	pMonster->SetTargetObj(pPlayerObj);
	pMonster->SetStartPosition(pMonster->GetPosition());

	// 몬스터 충돌 컴포넌트 생성 및 추가
	CColliderCircle* pMonsterCircleCollider = new CColliderCircle(pMonster);
	pMonsterCircleCollider->SetRadius(pMonsterCircleCollider->GetRadius() - 5.0f);
	m_vecMonster.push_back(pMonster);
	CreateObject(pMonster);
}
