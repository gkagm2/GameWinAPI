#include "stdafx.h"
#include "CAI.h"
#include "CAIState.h"

CAI::CAI() :
	m_pCurState(nullptr),
	m_pMonster(nullptr)
{
}

CAI::~CAI()
{
	unordered_map<wstring, CAIState*>::iterator iter = m_umapState.begin();
	for (; iter != m_umapState.end(); ++iter)
		delete iter->second;
}

void CAI::Update()
{
	if (nullptr != m_pCurState)
		m_pCurState->Update();
}

void CAI::LateUpdate()
{
	if (nullptr != m_pCurState)
		m_pCurState->LateUpdate();
}

void CAI::AddState(const wstring& _strKey, CAIState* _pState)
{
	_pState->m_pAI = this;
	_pState->SetName(_strKey);
	m_umapState.insert(make_pair(_strKey, _pState));
}

void CAI::ChangeState(const wstring& _strKey)
{
	if (nullptr != m_pCurState)
		m_pCurState->End();

	unordered_map<wstring, CAIState*>::iterator iter = m_umapState.find(_strKey);
	assert(iter != m_umapState.end() && L"Key의 State가 없음");
	m_pCurState = iter->second;
	m_pCurState->Start();
}