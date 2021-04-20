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
	// ���� ��ġ���� ��ǥ�� �ϴ� ������ �̵��Ѵ�.	
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();

}

void CGTA_WanderState::LateUpdate()
{

}

void CGTA_WanderState::Start()
{
	// TileMap�� �����´�.
	// ���� ��ġ�� �����´�.
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
	assert(pTileMap);


	//
	m_pTarget = CSceneManager::GetInstance()->GetCurScene()->FindObject(L"Player");

	

	// ���ο� ��ǥ�� �����Ѵ�.
	// ��ǥ�� Ÿ�� Ÿ���� wall�� water�� �ƴ� �����κ��� ���� �Ÿ� �̻� ������ �ִ� ������



}

void CGTA_WanderState::End()
{

}