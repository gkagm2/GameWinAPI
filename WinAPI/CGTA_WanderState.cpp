#include "stdafx.h"
#include "CGTA_WanderState.h"
#include "CGTA_AI.h"
#include "CTileMap.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"
#include "CGTA_Character.h"
#include "CTimeManager.h"

// Test code
#include "CCore.h"

CGTA_WanderState::CGTA_WanderState() :
	m_pTarget(nullptr),
	m_vStartPos{},
	m_vDestPos{},
	m_fPathFindCoolTime(0.f),
	m_fPathFindMaxCoolTime(3.f)
{
}

CGTA_WanderState::~CGTA_WanderState()
{
}
void CGTA_WanderState::Update()
{
	// 현재 위치에서 목표로 하는 곳으로 이동한다.	
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();

}

void CGTA_WanderState::LateUpdate()
{

}

void CGTA_WanderState::Start()
{
	// TileMap을 가져온다.
	// 현재 위치를 가져온다.
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
	assert(pTileMap);


	//
	m_pTarget = CSceneManager::GetInstance()->GetCurScene()->FindObject(L"Player");

	

	// 새로운 목표를 설정한다.
	// 목표는 타일 타입이 wall과 water가 아닌 곳으로부터 일정 거리 이상 떨어져 있는 곳으로



}

void CGTA_WanderState::End()
{

}