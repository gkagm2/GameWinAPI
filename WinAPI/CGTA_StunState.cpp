#include "stdafx.h"
#include "CGTA_StunState.h"
#include "CTimeManager.h"
#include "CGTA_Character.h"
#include "CGTA_Cop.h"
#include "CGTA_Citizen.h"

CGTA_StunState::CGTA_StunState() :
	m_fStunCoolTime(0.f),
	m_fStunMaxCoolTime(2.f)
{
}

CGTA_StunState::~CGTA_StunState()
{
}

void CGTA_StunState::Update()
{
	m_fStunCoolTime += DeltaTime;
	if (m_fStunCoolTime > m_fStunMaxCoolTime) {
		auto* pCop = dynamic_cast<CGTA_Cop*>(GetCharacter());
		if (pCop) {
			pCop->Trace();
			return;
		}
		auto* pCitizen = dynamic_cast<CGTA_Citizen*>(GetCharacter());
		if (pCitizen) {
			pCitizen->Runaway();
			return;
		}
	}
}

void CGTA_StunState::Start()
{
	m_fStunCoolTime = 0.f;
}

void CGTA_StunState::End()
{
}