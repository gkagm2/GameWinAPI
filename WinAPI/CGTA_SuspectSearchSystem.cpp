#include "stdafx.h"
#include "CGTA_SuspectSearchSystem.h"
#include "CResourceManager.h"
#include "CResource.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CGTA_Cop.h"
#include "CTimeManager.h"
#include "CCore.h"
#include "CTexture.h"

CGTA_SuspectSearchSystem::CGTA_SuspectSearchSystem(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_iKillCnt(0),
	m_iLevel(0),
	m_fUpdateCoolTime(0.f),
	m_fUpdateMaxCoolTime(1.f),
	m_eSearchLevel(E_SuspectSearchLevel::none)
{
}

CGTA_SuspectSearchSystem::~CGTA_SuspectSearchSystem()
{
}

void CGTA_SuspectSearchSystem::Init()
{
	CTexture* pTexture = CResourceManager::GetInstance()->GetTexture(STR_FILE_PATH_gta_CopFaceUI, STR_FILE_PATH_gta_CopFaceUI);
	SetTexture(pTexture);
	SetObjectName(STR_OBJECT_NAME_SuspectSearchSystem);
	SetScale(20.f, 20.f);
}

void CGTA_SuspectSearchSystem::Update()
{
	if (m_iKillCnt > 20) {
		ChangeLevel(E_SuspectSearchLevel::level3);
	}
	else if (m_iKillCnt > 10) {
		ChangeLevel(E_SuspectSearchLevel::level2);
	}
	else if (m_iKillCnt > 5) {
		ChangeLevel(E_SuspectSearchLevel::level1);
	}
	m_fUpdateCoolTime += DeltaTime;
	if (m_fUpdateCoolTime >= m_fUpdateMaxCoolTime) {
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
			}
		}
										 break;
		case E_SuspectSearchLevel::level3: {
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
			}
		}
										 break;
		case E_SuspectSearchLevel::level4:
			break;
		case E_SuspectSearchLevel::level5:

			break;
		}
		m_fUpdateCoolTime = 0.f;
	}
}

void CGTA_SuspectSearchSystem::Render(HDC _hDC)
{
	if (nullptr == GetTexture())
		return;
	Vector2 vResolution = CCore::GetInstance()->GetResolution();

	Vector3 vPos{};
	vPos.x = vResolution.x * 0.5f;
	vPos.y = 5.f;

	for (int i = 0; i < (int)m_eSearchLevel; ++i) {
		int iNextOff = i * GetScale().x;
		TransparentBlt(_hDC, vPos.x + iNextOff, vPos.y, GetScale().x, GetScale().y, GetTexture()->GetDC(), 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), EXCEPTION_COLOR_RGB_BLACK);
	}
}