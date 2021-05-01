#include "stdafx.h"
#include "CGTA_SuspectSearchSystem.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CGTA_Cop.h"

CGTA_SuspectSearchSystem::CGTA_SuspectSearchSystem(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_iKiilCnt(0),
	m_iLevel(0),
	m_fInitCoolTime(0.f),
	m_fInitMaxCoolTime(15.f),
	m_eSearchLevel(E_SuspectSearchLevel::none)
{
}

CGTA_SuspectSearchSystem::~CGTA_SuspectSearchSystem()
{
}

void CGTA_SuspectSearchSystem::Init()
{
}

void CGTA_SuspectSearchSystem::Update()
{
	switch (m_eSearchLevel) {
	case E_SuspectSearchLevel::none:
		break;
	case E_SuspectSearchLevel::level1:
	{
		// every cops trace suspect
		vector<CObject*>& vecObjs = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::CITIZEN);
		for (UINT i = 0; i < vecObjs.size(); ++i) {
			CGTA_Cop* pCop = dynamic_cast<CGTA_Cop*>(vecObjs[i]);
			if (nullptr == pCop)
				continue;

			if (E_AIState::trace != pCop->GetAIState() &&
				E_AIState::dead != pCop->GetAIState() &&
				E_AIState::drive != pCop->GetAIState()) {
				pCop->Trace();
			}
		}
	}
		break;
	case E_SuspectSearchLevel::level2: {
		// every cops trace suspect and has pistol
		vector<CObject*>& vecObjs = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::CITIZEN);
		for (UINT i = 0; i < vecObjs.size(); ++i) {
			CGTA_Cop* pCop = dynamic_cast<CGTA_Cop*>(vecObjs[i]);
			if (nullptr == pCop)
				continue;

			if (E_AIState::dead == pCop->GetAIState())
				continue;

			if (E_AIState::trace != pCop->GetAIState() &&
				E_AIState::drive != pCop->GetAIState()) {
				pCop->Trace();
			}
			if (E_WeaponType::FIST == pCop->GetCurWeaponType()) {
				if (false == pCop->IsWeaponExists(E_WeaponType::PISTOL)) {
					TWeaponInfo tPistolWeapon;
					tPistolWeapon.InitWeapon(E_WeaponType::PISTOL);
					tPistolWeapon.bIsInfinite = true;
					pCop->SetWeaponState(true, E_WeaponType::PISTOL);
					pCop->SetWeaponInfo(E_WeaponType::PISTOL, tPistolWeapon);
				}
				pCop->ChangeNextWeapon();
			}
		}
	}
		break;
	case E_SuspectSearchLevel::level3:
		// TODO : SDT
		break;
	case E_SuspectSearchLevel::level4:
		break;
	case E_SuspectSearchLevel::level5:

		break;
	}
}

void CGTA_SuspectSearchSystem::Render(HDC _hDC)
{
}

void CGTA_SuspectSearchSystem::ChangeLevel(E_SuspectSearchLevel _eLevel)
{
}
