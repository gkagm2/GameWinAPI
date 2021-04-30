#include "stdafx.h"

#include"CEventManager.h"

#include "CAIState.h"
#include "CMonster.h"
#include "C_AI.h"

CAIState::CAIState() :
	m_pAI(nullptr)
{
}

CAIState::~CAIState()
{
}

void CAIState::ChangeState(const wstring& _strNextState)
{
	tEvent evn = {};
	evn.eType = E_EventType::CHANGE_STATE;
	evn.lParam = (DWORD_PTR)new wstring(_strNextState);
	evn.wParam = (DWORD_PTR)m_pAI;
	CEventManager::GetInstance()->AddEvent(evn);
}

CMonster* CAIState::GetMonster()
{
	return m_pAI->GetMonster();
}
