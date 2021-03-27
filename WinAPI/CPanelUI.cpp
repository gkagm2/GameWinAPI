#include "stdafx.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CImageUI.h"
#include "CResourceManager.h"
#include "CTexture.h"

#include "CDebug.h"

CPanelUI::CPanelUI(E_GroupType _eGroupType = E_GroupType::UI) :
	CUI(_eGroupType),
	m_fTopPadding(0.f),
	m_fBottomPadding(0.f),
	m_fLeftPadding(0.f),
	m_fRightPadding(0.f)
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

	SetTexture(pTileTexture);

	CImageUI* pChildImageUI = new CImageUI(E_GroupType::UI);
	//pChildImageUI->SetTexture(pTileTexture);
	pChildImageUI->SetPosition(150.0f, 150.0f, 0.0f);
	pChildImageUI->SetObjectName(L"Image UI");
	pChildImageUI->SetLT(Vector2{});
	AddChildUI((CUI*)pChildImageUI);
	

	CImageUI* pChild2 = new CImageUI(E_GroupType::UI);
	//pChild2->SetTexture(pTileTexture);
	pChild2->SetPosition(30.0f, 50.0f, 0.0f);
	pChild2->SetObjectName(L"Image UI");
	pChild2->SetLT(Vector2{});
	//pChildImageUI->AddChildUI((CUI*)pChild2);
	AddChildUI((CUI*)pChild2);

	float fPadding[4]{5.f, 5.f, 5.f, 5.f}; // l, t, r, b
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
	Vector3 vFinalPos = GetFinalPosition();
	Vector3 vScale = GetScale();
	Debug->Print(vFinalPos + 30.0f, L"sddi", GetObjectName().c_str(), vFinalPos.x, vFinalPos.y, m_bIsOn);

	CTexture* pTexture = GetTexture();
	if (nullptr == pTexture)
		CUI::Render(_hDC);
	else {
		Vector3 vFinalPos = GetFinalPosition();

		BitBlt(_hDC, vFinalPos.x, vFinalPos.y, GetScale().x, GetScale().y
			, pTexture->GetDC(), 0, 0, SRCCOPY);
	}

	const vector<CUI*>& vecChildUI = GetChildsUI();
	for (UINT i = 0; i < vecChildUI.size(); ++i)
		vecChildUI[i]->Render(_hDC);
}

void CPanelUI::SetPadding(float _fLeft, float _fTop, float _fRight, float _fBottom)
{
	m_fLeftPadding = _fLeft;
	m_fTopPadding = _fTop;
	m_fRightPadding = _fRight;
	m_fBottomPadding = _fBottom;
}