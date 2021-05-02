#include "stdafx.h"
#include "CGTA_AI.h"

#include "CSceneManager.h"
#include "CScene.h"
#include "CGTA_AIState.h"
#include "CGTA_Character.h"
#include "CTileMap.h"
#include "CObject.h"
#include "CTimeManager.h"
#include "CCamera.h"

#include "CKeyManager.h"
#include "CDebug.h"

CGTA_AI::CGTA_AI() :
	m_pCurState(nullptr),
	m_pCharacter(nullptr),
	m_bIsRender(false),
	m_bIsActive(true)
{
}

CGTA_AI::CGTA_AI(const CGTA_AI& _origin) :
	m_pCurState(nullptr),
	m_pCharacter(nullptr),
	m_bIsRender(false),
	m_bIsActive(true)
{
	unordered_map<wstring, CGTA_AIState*>::const_iterator iter = _origin.m_umapState.begin();

	for (; iter != _origin.m_umapState.end(); ++iter) {
		wstring strName = iter->first;
		auto state = iter->second->Clone();
		state->SetAI(this);
		m_umapState.insert(make_pair(strName, state));
	}
}

void CGTA_AI::Move(float _fSpeed)
{
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
	// 경로에 따라 이동한다.
	list<TTilePos>& path = m_pCharacter->GetPathFinding()->GetPath();
	// 목적지에 도착했는지 확인한다.
	// 목적지에 도착했으면
	if (m_pCharacter->GetPathFinding()->IsArrivedDestination()) {
		return;
	}
	else { // 목적지에 도착하지 않았으면
		// 다음에 가야 할 위치를 가져온다.
		Vector2 vCurPos = m_pCharacter->GetPosition();

		// 위치가 같다면
		Vector2 vNextPos = pTileMap->TilePosToVector(path.front());
		vNextPos += TILE_SIZE * 0.5f;

		if (abs(vNextPos.x - vCurPos.x) < 15 && abs(vNextPos.y - vCurPos.y) < 15) {
			path.pop_front(); // 팝해준다.
		}
		else { // 위치가 같지 않다면
			// 그 위치로 이동한다.
			// 1.  방향 구하기
			Vector2 vDir = Vector2(vNextPos.x - vCurPos.x, vNextPos.y - vCurPos.y);
			vDir.Normalized();

			// 이동
			m_pCharacter->SetPosition(m_pCharacter->GetPosition() + vDir * _fSpeed * DeltaTime);
		}
	}
}

void CGTA_AI::RotateBody()
{
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
	// 몸 회전
	TTilePos tNextPathPos = GetCharacter()->GetPathFinding()->GetNextPath();
	Vector2 vNextPathPos = pTileMap->TilePosToVector(tNextPathPos);
	vNextPathPos = vNextPathPos + TILE_SIZE * 0.5f;

	Vector3 vNextPathDir = vNextPathPos - GetCharacter()->GetPosition();
	vNextPathDir.Normalized();

	Vector3 vHeadDir = GetCharacter()->GetUpVector();
	vHeadDir *= -1;

	Vector3 vCross = CMyMath::GetCross(vNextPathDir, vHeadDir);
	if (vCross.z < 0) {
		if (vCross.z < -0.1f)
			GetCharacter()->RotateRP(vNextPathDir, vHeadDir, 250.f * DeltaTime);
		else
			GetCharacter()->RotateRP(vNextPathDir, vHeadDir, 100.f * DeltaTime);
	}
	else if (vCross.z > 0) {
		if (vCross.z > 0.1f)
			GetCharacter()->RotateRP(vNextPathDir, vHeadDir, 250.f * DeltaTime);
		else
			GetCharacter()->RotateRP(vNextPathDir, vHeadDir, 100.f * DeltaTime);
	}
			

	//GetCharacter()->LookAt(vNextPathPos, 200.f);
}

void CGTA_AI::Render(HDC _hDC)
{
	if (false == IsActive())
		return;
	if (false == IsRender())
		return;

	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
	list<TTilePos>& path = m_pCharacter->GetPathFinding()->GetPath();

	list<TTilePos>::iterator iter = path.begin();
	if (m_pCharacter->GetPathFinding()->IsArrivedDestination())
		return;
	if (iter == path.end())
		return;
	TTilePos prev = *iter;

	++iter;
	for (; iter != path.end(); ++iter) {
		Vector2 vCurPos = MainCamera->GetRenderPosition(pTileMap->TilePosToVector(prev));
		vCurPos += (TILE_SIZE * 0.5f);
		Rectangle(_hDC, (int)vCurPos.x - 5, (int)vCurPos.y - 5, (int)vCurPos.x + 5, (int)vCurPos.y + 5);
		MoveToEx(_hDC, (int)vCurPos.x, (int)vCurPos.y, nullptr);
		Vector2 destPos = MainCamera->GetRenderPosition(pTileMap->TilePosToVector(*iter));

		destPos += (TILE_SIZE * 0.5f);
		LineTo(_hDC, (int)destPos.x, (int)destPos.y);
		Ellipse(_hDC, (int)destPos.x - 5, (int)destPos.y - 5, (int)destPos.x + 5, (int)destPos.y + 5);
		prev = *iter;
	}
}

CGTA_AI::~CGTA_AI()
{
	unordered_map<wstring, CGTA_AIState*>::iterator iter = m_umapState.begin();
	for (; iter != m_umapState.end(); ++iter)
		delete iter->second;
}
void CGTA_AI::Update()
{
	if (false == IsActive())
		return;

#pragma region TEST_CODE
	if (Debug->IsShow())
		SetRender(true);
	else
		SetRender(false);
#pragma endregion

	if (nullptr != m_pCurState)
		m_pCurState->Update();
}

void CGTA_AI::LateUpdate()
{
	if (false == IsActive())
		return;

	if (nullptr != m_pCurState)
		m_pCurState->LateUpdate();
}

void CGTA_AI::AddState(const wstring& _strKey, CGTA_AIState* _pState)
{
	_pState->SetAI(this);
	_pState->SetName(_strKey);
	m_umapState.insert(make_pair(_strKey, _pState));
}

void CGTA_AI::ChangeState(const wstring& _strKey)
{
	if (nullptr != m_pCurState)
	
		m_pCurState->End();

	unordered_map<wstring, CGTA_AIState*>::iterator iter = m_umapState.find(_strKey);
	assert(iter != m_umapState.end() && L"Key의 State가 없음");
	m_pCurState = iter->second;
	m_pCurState->Start();
}