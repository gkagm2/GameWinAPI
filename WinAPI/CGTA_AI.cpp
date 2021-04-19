#include "stdafx.h"
#include "CGTA_AI.h"
#include "CGTA_AIState.h"

CGTA_AI::CGTA_AI() :
	m_pCurState(nullptr),
	m_pCharacter(nullptr)
{
}

CGTA_AI::~CGTA_AI()
{
	unordered_map<wstring, CGTA_AIState*>::iterator iter = m_umapState.begin();
	for (; iter != m_umapState.end(); ++iter)
		delete iter->second;
}
void CGTA_AI::Update()
{
	if (nullptr != m_pCurState)
		m_pCurState->Update();
}

void CGTA_AI::LateUpdate()
{
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