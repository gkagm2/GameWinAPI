#include "stdafx.h"

#include "CScene.h"
#include "CSceneManager.h"

#include "CMonster_AI.h"
#include "C_AI.h"
#include "CAIState.h"
#include "CIdleState.h"
#include "CTraceState.h"

#include "CCamera.h"
#include "CDebug.h"

CMonster_AI::CMonster_AI(E_GroupType _eGroupType) :
	CMonster(_eGroupType)
{
	CreateAI();

	GetAI()->AddState(L"idle", new CIdleState);
	GetAI()->AddState(L"trace", new CTraceState);
	GetAI()->ChangeState(L"idle");
}

CMonster_AI::~CMonster_AI()
{
	if (nullptr != m_pAI)
		delete m_pAI;
}

void CMonster_AI::Update()
{
	m_pAI->Update();
}

void CMonster_AI::LateUpdate()
{
	m_pAI->LateUpdate();
	__super::LateUpdate();
}

void CMonster_AI::Render(HDC _hDC)
{
	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hDC, myBrush);

	Vector3 vRenderPos = MainCamera->GetRenderPosition(GetPosition());
	Ellipse(_hDC, int(vRenderPos.x - 200.f), int(vRenderPos.y - 200.f), int(vRenderPos.x + 200.f), int(vRenderPos.y + 200.f));

	SelectObject(_hDC, oldBrush);
	DeleteObject(myBrush);
	__super::Render(_hDC);
}