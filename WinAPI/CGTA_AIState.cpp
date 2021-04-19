#include "stdafx.h"
#include "CEventManager.h"
#include "CGTA_AIState.h"
#include "CGTA_Character.h"
#include "CAI.h"
CGTA_AIState::CGTA_AIState() :
	m_pAI(nullptr)
{
}

CGTA_AIState::~CGTA_AIState()
{
}

void CGTA_AIState::ChangeState(const wstring& _strNextState)
{
	tEvent evn = {};
	evn.eType = E_EventType::CHANGE_STATE;
	evn.lParam = (DWORD_PTR)new wstring(_strNextState);
	evn.wParam = (DWORD_PTR)m_pAI;
	CEventManager::GetInstance()->AddEvent(evn);
}

CGTA_Character* CGTA_AIState::GetCharacter()
{
	return nullptr;
}