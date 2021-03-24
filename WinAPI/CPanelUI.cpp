#include "stdafx.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CImageUI.h"
#include "CResourceManager.h"


CPanelUI::CPanelUI(E_GroupType _eGroupType = E_GroupType::UI) :
	CUI(_eGroupType)
{
}

CPanelUI::~CPanelUI()
{
}

void CPanelUI::Init()
{
	SetPosition(Vector3{});
	SetScale(Vector3{ 300.f,300.f,0.f });
	SetObjectName(L"ParentUI");

	// UI 초기화
	/*CUI* pChildUI = new CUI(E_GroupType::UI);
	pChildUI->SetPosition(Vector3{150.0f, 150.0f, 0.f});
	pChildUI->SetScale(Vector3{ 100.f, 100.f, 0.f });
	pChildUI->SetObjectName(L"ChildUI");
	AddChildUI(pChildUI);*/

	// Image Texture 가져옴
	CTexture* pTileTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_Tile);
	if (nullptr == pTileTexture) {
		pTileTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_Tile, STR_FILE_PATH_Tile);
	}

	CImageUI* pChildImageUI = new CImageUI(E_GroupType::UI);
	pChildImageUI->SetTexture(pTileTexture);
	pChildImageUI->SetPosition(150.0f, 150.0f, 0.0f);
	pChildImageUI->SetObjectName(L"Image UI");
	AddChildUI((CUI*)pChildImageUI);
}

void CPanelUI::Update()
{
	CUI::Update();
}

void CPanelUI::LateUpdate()
{
	CUI::LateUpdate();
}

void CPanelUI::Render(HDC _hDC)
{
	CUI::Render(_hDC);
}