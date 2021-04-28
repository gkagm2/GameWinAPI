#include "stdafx.h"
#include "CGTA_PlayerUI.h"
#include "CCore.h"
#include "CResource.h"
#include "CResourceManager.h"
#include "CImageUI.h"
#include "CTexture.h"

CGTA_PlayerUI::CGTA_PlayerUI(E_GroupType _eGroupType) :
	CUI(_eGroupType),
	m_pWeaponImage(nullptr),
	m_pLifeImage(nullptr),
	m_fLife(0.f)
{
}

CGTA_PlayerUI::~CGTA_PlayerUI()
{
}

void CGTA_PlayerUI::Init()
{
	POINT rect = CCore::GetInstance()->GetResolution();
	SetPosition(rect.x - 300, 0);
	
	// Weapon Image Setting
	m_pWeaponImage = new CImageUI(E_GroupType::UI);
	m_pWeaponImage->Init();
	CreateObject(m_pWeaponImage);


	for (int i = 0; i < 5; ++i) {
		// Life Image Setting
		CImageUI* pLifeImage = new CImageUI(E_GroupType::UI);
		pLifeImage->Init();

		CTexture* pLifeTex = CResourceManager::GetInstance()->FindTexture(STR_FILE_PATH_UI_GTA_Life);
		if (nullptr == pLifeTex) {
			pLifeTex = CResourceManager::GetInstance()->LoadTexture(STR_FILE_PATH_UI_GTA_Life, STR_FILE_PATH_UI_GTA_Life);
			assert(pLifeTex);
		}
		pLifeImage->SetTexture(pLifeTex);
		pLifeImage->SetImageTransPaBlt(Vector2{ 0,0 }, Vector2{ pLifeTex->GetWidth(), pLifeTex->GetHeight() }, EXCEPTION_COLOR_RGB_BLACK);
		CreateObject(pLifeImage);
	}
}

void CGTA_PlayerUI::Update()
{
	CUI::Update();
}

void CGTA_PlayerUI::Render(HDC _hDC)
{
	if (nullptr == GetTexture())
		return;

	Vector3 vFinalPos = GetFinalPosition();
	Vector3 vScale = GetScale();

	wchar_t buff[255];
	TextOut(_hDC, vFinalPos.x, vFinalPos.y + 150, m_tWeaponInfo.strName.c_str(), wcslen(m_tWeaponInfo.strName.c_str()));

	swprintf(buff, 255, L"Ammo : %d", m_tWeaponInfo.iBulletCnt);
	TextOut(_hDC, vFinalPos.x, vFinalPos.y + 180, buff, wcslen(buff));

	// Weapon Texture 
	//TransparentBlt(_hDC, (int)vFinalPos.x, (int)vFinalPos.y, (int)GetScale().x, (int)GetScale().y, m_pWeaponTexture->GetDC(), 0, 0, m_pWeaponTexture->GetWidth(), m_pWeaponTexture->GetHeight(), 0);
	
	CUI::Render(_hDC);
}

void CGTA_PlayerUI::SetLifeUI(float _fHP)
{
	// 체력2일경우 하트가 한개임
	m_fLife = _fHP;
}

void CGTA_PlayerUI::ChangeWeaponUI(const TWeaponInfo& _tWeaponInfo)
{
	m_tWeaponInfo = _tWeaponInfo;
	if (m_tWeaponInfo.strName.compare(STR_NAME_Fist) == 0) {
		m_pWeaponImage->SetTexture(nullptr);
		return;
	}
	wstring strPath = STR_FILE_PATH_GTA_Texture + m_tWeaponInfo.strName + L".bmp";
	CTexture* pWeaponTex = CResourceManager::GetInstance()->FindTexture(strPath);
	if (nullptr == pWeaponTex) {
		pWeaponTex = CResourceManager::GetInstance()->LoadTexture(strPath, strPath);
		assert(pWeaponTex);
	}
	m_pWeaponImage->SetTexture(pWeaponTex);
	m_pWeaponImage->SetImageTransPaBlt(Vector2{ 0,0 }, Vector2{ pWeaponTex->GetWidth(), pWeaponTex->GetHeight() }, EXCEPTION_COLOR_RGB_BLACK);
}