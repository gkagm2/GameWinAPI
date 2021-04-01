#include "stdafx.h"

#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CScene.h"

#include "CTraceState.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"



CTraceState::CTraceState() :
	m_pTarget(nullptr)
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::Update()
{
	Vector3 vDiff = GetMonster()->GetPosition() - m_pTarget->GetPosition();
	float fDistance = vDiff.GetDistance();
	if (200.f < fDistance) {
		ChangeState(L"idle");
	}

	Vector3 vDir = m_pTarget->GetPosition() - GetMonster()->GetPosition();
	vDir.Normalized();
	Vector3 vCurPos = GetMonster()->GetPosition() + vDir * 100.f * DeltaTime;
	GetMonster()->SetPosition(vCurPos);
}

void CTraceState::LateUpdate()
{
}

void CTraceState::Start()
{
	if (nullptr == m_pTarget) {
		CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
		if (pCurScene)
			m_pTarget = pCurScene->FindObject(L"Player");
	}
}

void CTraceState::End()
{
}