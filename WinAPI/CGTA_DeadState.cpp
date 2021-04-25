#include "CGTA_DeadState.h"
#include "CTimeManager.h"
#include "CGTA_Character.h"
#include "CGTA_AI.h"

CGTA_DeadState::CGTA_DeadState() :
	m_fDeadCoolTime(0.f),
	m_fDeadMaxCoolTime(120.f)
{
}

CGTA_DeadState::~CGTA_DeadState()
{
}

void CGTA_DeadState::Update()
{
	m_fDeadCoolTime += DeltaTime;
	if (m_fDeadCoolTime >= m_fDeadMaxCoolTime) {
		// ���º�ȯ
		GetAI()->ChangeState(L"wander");
		// TODO : info �ʱ�ȭ

		m_fDeadCoolTime = 0.f;
	}
}

void CGTA_DeadState::Start()
{

}

void CGTA_DeadState::End()
{

}