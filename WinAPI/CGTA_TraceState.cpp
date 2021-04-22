#include "stdafx.h"
#include "CGTA_TraceState.h"
#include "CResourceManager.h"
#include "CResource.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"
#include "CGTA_Player.h"
#include "CTimeManager.h"

CGTA_TraceState::CGTA_TraceState() :
	m_pTarget(nullptr),
	m_fDistance(200.f)
{
}

CGTA_TraceState::~CGTA_TraceState()
{
}

void CGTA_TraceState::Update()
{
	if (nullptr == m_pTarget)
		return;

	Vector3 vDiff = GetCharacter()->GetPosition() - m_pTarget->GetPosition();
	float fDistance = vDiff.GetDistance();
	if (fDistance > fDistance) {
		ChangeState(L"wander");
	}

	Vector3 vDir = m_pTarget->GetPosition() - GetCharacter()->GetPosition();
	vDir.Normalized();
	Vector3 vCurPos = GetCharacter()->GetPosition() + vDir * GetCharacter()->CharacterInfo().fMoveSpeed * DeltaTime;


	m_pTarget->GetPosition();
}

void CGTA_TraceState::LateUpdate()
{
}

void CGTA_TraceState::Start()
{

}

void CGTA_TraceState::End()
{
}