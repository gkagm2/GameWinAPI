#include "stdafx.h"	
#include "CSceneManager.h"
#include "CScene.h"
#include "CGTA_UIContainer.h"
#include "CGTA_PlayerUI.h"

CGTA_UIContainer::CGTA_UIContainer(E_GroupType _eGroupType) :
	CUI(_eGroupType),
	m_pPlayerUI(nullptr)
{
}

CGTA_UIContainer::~CGTA_UIContainer()
{
	m_pPlayerUI = nullptr;
}

void CGTA_UIContainer::Init()
{
	SetObjectName(STR_OBJECT_NAME_UIContainer);
	m_pPlayerUI = new CGTA_PlayerUI(E_GroupType::UI);
	m_pPlayerUI->Init();
	CSceneManager::GetInstance()->GetCurScene()->AddObject(m_pPlayerUI);
}

void CGTA_UIContainer::Update()
{
}

void CGTA_UIContainer::Render(HDC _hDC)
{
}