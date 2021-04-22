#include "stdafx.h"
#include "CGTA_AI.h"
#include "CGTA_AIState.h"



CGTA_AI::CGTA_AI() :
	m_pCurState(nullptr),
	m_pCharacter(nullptr)
{
}

CGTA_AI::CGTA_AI(const CGTA_AI& _origin) :
	m_pCurState(nullptr),
	m_pCharacter(nullptr)
{
	unordered_map<wstring, CGTA_AIState*>::const_iterator iter = _origin.m_umapState.begin();

	for (; iter != _origin.m_umapState.end(); ++iter) {
		wstring strName = iter->first;
		auto state = iter->second->Clone();
		state->SetAI(this);
		m_umapState.insert(make_pair(strName, state));
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
	assert(iter != m_umapState.end() && L"Key�� State�� ����");
	m_pCurState = iter->second;
	m_pCurState->Start();
}