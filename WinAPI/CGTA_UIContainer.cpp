#include "stdafx.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CGTA_UIContainer.h"
#include "CGTA_WeaponUI.h"

CGTA_UIContainer::CGTA_UIContainer(E_GroupType _eGroupType) :
	CUI(_eGroupType),
	m_pWeaponUI(nullptr)
{
}

CGTA_UIContainer::~CGTA_UIContainer()
{
	m_pWeaponUI = nullptr;
}

void CGTA_UIContainer::Init()
{
	SetObjectName(STR_OBJECT_NAME_UIContainer);
	m_pWeaponUI = new CGTA_WeaponUI(E_GroupType::UI);
	m_pWeaponUI->Init();
	CSceneManager::GetInstance()->GetCurScene()->AddObject(m_pWeaponUI);
}

void CGTA_UIContainer::Update()
{
}

void CGTA_UIContainer::Render(HDC _hDC)
{
}