#include "stdafx.h"

#include "CSceneManager.h"
#include "CScene.h"

#include "CIdleState.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CMonster_AI.h"

CIdleState::CIdleState() :
	m_pTarget(nullptr)
{
	
}

CIdleState::~CIdleState()
{
}

void CIdleState::Update()
{
	assert(m_pTarget);
	Vector2 vDiff = m_pTarget->GetPosition() - GetMonster()->GetPosition();
	float fDistance = vDiff.GetDistance();

	if (200.f >= fDistance)
		ChangeState(L"trace");
}

void CIdleState::LateUpdate()
{
}

void CIdleState::Start()
{
	if (nullptr == m_pTarget) {
		CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
		if (pCurScene)
			m_pTarget = pCurScene->FindObject(STR_OBJECT_NAME_Player);
	}
}

void CIdleState::End()
{
}