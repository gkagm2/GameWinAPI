#include "stdafx.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CImageUI.h"
#include "CResourceManager.h"
#include "CTexture.h"

#include "CKeyManager.h"

#include "CDebug.h"

CPanelUI::CPanelUI(E_GroupType _eGroupType = E_GroupType::UI) :
	CUI(_eGroupType),
	m_fTopPadding(0.f),
	m_fBottomPadding(0.f),
	m_fLeftPadding(0.f),
	m_fRightPadding(0.f),
	m_vSpacing{},
	m_iColCnt(0),
	m_iRowCnt(0),
	m_vOriginalPos{},
	m_vDiff{}
{
}

CPanelUI::~CPanelUI()
{
}

void CPanelUI::Init()
{
	CTexture* pTileTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_UI_BG);
	if (nullptr == pTileTexture)
		pTileTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_UI_BG, STR_FILE_PATH_UI_BG);
	SetTexture(pTileTexture);
}

void CPanelUI::Update()
{
	if (m_bIsDown) {
		m_vDiff = Vector3(MousePosition.x, MousePosition.y) - m_vOriginalPos;
		SetPosition(m_vDiff);
	}
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
	if (GetObjectName() == L"ParentUI") {
		Debug->Print(Vector2(60, 60), L"s", GetObjectName().c_str());
	}
	else {
		Debug->Print(Vector2(60, 90), L"s", GetObjectName().c_str());
	}
	

	CTexture* pTexture = GetTexture();
	if (nullptr == pTexture)
		CUI::Render(_hDC);
	else {
		Vector3 vFinalPos = GetFinalPosition();

		BitBlt(_hDC, (int)vFinalPos.x, (int)vFinalPos.y, (int)GetScale().x, (int)GetScale().y
			, pTexture->GetDC(), 0, 0, SRCCOPY);
	}

	const vector<CUI*>& vecChildUI = GetChildsUI();
	for (UINT i = 0; i < vecChildUI.size(); ++i)
		vecChildUI[i]->Render(_hDC);
}

void CPanelUI::OnPointerDown()
{
	m_vOriginalPos = MousePosition - GetFinalPosition();
}

void CPanelUI::OnPointerUp()
{
}

void CPanelUI::SetPadding(float _fLeft, float _fTop, float _fRight, float _fBottom)
{
	m_fLeftPadding = _fLeft;
	m_fTopPadding = _fTop;
	m_fRightPadding = _fRight;
	m_fBottomPadding = _fBottom;
}

void CPanelUI::SetGridUI(int _iColCnt = 1, int _iRowCnt = 1, float _fLeftPadding =2.f, float _fTopPadding = 2.f, float _fRightPadding = 2.f, float _fBottomPadding =2.f, float _fSpacingX =2.f, float _fSpacingY = 2.f)
{
	SetGrid(_iColCnt, _iRowCnt);
	SetPadding(_fLeftPadding, _fTopPadding, _fRightPadding, _fBottomPadding);
	SetSpacing(_fSpacingX, _fSpacingY);

	float realWidth = (GetScale().x - (m_fLeftPadding + m_fRightPadding));
	float realHeight = (GetScale().y - (m_fTopPadding + m_fBottomPadding));
	float fCellWidth = realWidth / m_iColCnt;
	float fCellHeight = realHeight / m_iRowCnt;
	float resultX = realWidth - (m_iColCnt - 1) * m_vSpacing.x;
	float resultY = realHeight - (m_iRowCnt - 1) * m_vSpacing.y;
	fCellWidth = resultX / m_iColCnt;
	fCellHeight = resultY / m_iRowCnt;
	if (fCellWidth > fCellHeight)
		fCellWidth = fCellHeight;
	else
		fCellHeight = fCellWidth;
	float startPosx = GetPosition().x + m_fLeftPadding;
	float startPosy = GetPosition().y + m_fTopPadding;

	CTexture* pTileTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_Tile);
	if (nullptr == pTileTexture)
		pTileTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_Tile, STR_FILE_PATH_Tile);

	for (int i = 0; i < m_iRowCnt; ++i) {
		startPosx = GetFinalPosition().x + m_fLeftPadding;
		for (int j = 0; j < m_iColCnt; ++j) {
			// 이미지 생성
			CImageUI* pChildImageUI = new CImageUI(E_GroupType::UI);
			//pChildImageUI->SetTexture(pTileTexture);
			pChildImageUI->SetPosition(startPosx, startPosy, 0.0f);
			pChildImageUI->SetScale(fCellWidth, fCellHeight);
			pChildImageUI->SetLT(Vector2{});
			AddChildUI((CUI*)pChildImageUI);
			startPosx += fCellWidth + m_vSpacing.x;
		}
		startPosy += fCellHeight + m_vSpacing.y;
	}
}
