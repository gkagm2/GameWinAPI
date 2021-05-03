#include "stdafx.h"
#include "CGTA_PlayerUI.h"
#include "CCore.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CResource.h"
#include "CResourceManager.h"
#include "CImageUI.h"
#include "CTextUI.h"
#include "CTexture.h"
#include "CGTA_Player.h"

CGTA_PlayerUI::CGTA_PlayerUI(E_GroupType _eGroupType) :
	CUI(_eGroupType),
	m_vLifeScale{30.f, 30.f, 0.f},
	m_pWeaponImage(nullptr),
	m_fLife(0.f),
	m_pMoneyTextUI(nullptr),
	m_pAmmoTextUI(nullptr),
	m_pPlayer(nullptr)
{
}

CGTA_PlayerUI::~CGTA_PlayerUI()
{
}

void CGTA_PlayerUI::Init()
{
	POINT tResolution = CCore::GetInstance()->GetResolution();
	SetPosition(0, 0);
	
	//// Weapon Image Setting
	m_pWeaponImage = new CImageUI(E_GroupType::UI);
	m_pWeaponImage->Init();
	m_pWeaponImage->SetPosition((float)(tResolution.x - 250.f) , 60.f, 0.f);
	m_pWeaponImage->SetScale(150, 150, 0);
	CreateObject(m_pWeaponImage);

	// Life Image Setting
	for (int i = 0; i < 5; ++i) {
		// Life Image Setting
		CImageUI* pLifeImage = new CImageUI(E_GroupType::UI);
		pLifeImage->Init();

		CTexture* pLifeTex = CResourceManager::GetInstance()->GetTexture(STR_FILE_PATH_UI_GTA_Life, STR_FILE_PATH_UI_GTA_Life);
		pLifeImage->SetTexture(pLifeTex);
		pLifeImage->SetImageTransPaBlt(Vector2{ 0,0 }, Vector2{ pLifeTex->GetWidth(), pLifeTex->GetHeight() }, m_vLifeScale, EXCEPTION_COLOR_RGB_BLACK);
		Vector3 vPosition = {(float)(tResolution.x -250 +  i * pLifeImage->ScaleX() + 10), 20.f, 0.f };
		pLifeImage->SetPosition(vPosition);
		CreateObject(pLifeImage);
		m_vecLifeImages.push_back(pLifeImage);
	}

	// Ammo Text Setting
	m_pAmmoTextUI = new CTextUI(E_GroupType::UI);
	m_pAmmoTextUI->SetPosition((float)tResolution.x - 220.f, 180.f);
	m_pAmmoTextUI->SetScale(40, 40, 0);
	CreateObject(m_pAmmoTextUI);

	// Money Text Setting
	m_pMoneyTextUI = new CTextUI(E_GroupType::UI);
	m_pMoneyTextUI->SetPosition((float)tResolution.x - 250.f , 60.f);
	m_pMoneyTextUI->SetScale(30, 30, 0);
	CreateObject(m_pMoneyTextUI);
}

void CGTA_PlayerUI::Update()
{
	// Display Player info
	CUI::Update();
}

void CGTA_PlayerUI::Render(HDC _hDC)
{
	if (nullptr == m_pPlayer) {
		m_pPlayer = (CGTA_Player*)CSceneManager::GetInstance()->GetCurScene()->FindObject(STR_OBJECT_NAME_Player);
		assert(m_pPlayer);
	}

	Vector3 vFinalPos = GetFinalPosition();
	Vector3 vScale = GetScale();


	// Render Money
	int iMoney = m_pPlayer->GetMoney();
	m_pMoneyTextUI->SetText(L"$" + std::to_wstring(iMoney));

	// Render Weapon ammo
	if (E_WeaponType::FIST == m_pPlayer->GetCurWeaponType())
		m_pAmmoTextUI->SetRender(false);
	else {
		m_pAmmoTextUI->SetRender(true);
		m_pAmmoTextUI->SetText(m_pPlayer->GetWeaponInfo(m_pPlayer->GetCurWeaponType()).iBulletCnt);
	}
	

	// Render Player HP
	float fHp = m_pPlayer->CharacterInfo().fHp;
	float fMaxHp = m_pPlayer->CharacterInfo().fMaxHp;
	float fOneLifeImg = fMaxHp / m_vecLifeImages.size();
	float fLifeImgCnt = fHp / fOneLifeImg;
	for (int i = 0; i < m_vecLifeImages.size(); ++i) {
		if (i < fLifeImgCnt)
			m_vecLifeImages[i]->SetRender(true);
		else
			m_vecLifeImages[i]->SetRender(false);
	}

	// None
	//CUI::Render(_hDC);
}
	
void CGTA_PlayerUI::SetLifeUI(float _fHP)
{
	// 체력2일경우 하트가 한개임
	m_fLife = _fHP;
}

void CGTA_PlayerUI::ChangeWeaponUI(const TWeaponInfo& _tWeaponInfo)
{
	if (_tWeaponInfo.strName.compare(STR_NAME_Fist) == 0) {
		m_pWeaponImage->SetTexture(nullptr);
		return;
	}
	// TODO : .bmp를 string table에 넣기
	wstring strPath = STR_FILE_PATH_GTA_Texture + _tWeaponInfo.strName + L".bmp";
	CTexture* pWeaponTex = CResourceManager::GetInstance()->GetTexture(strPath, strPath);
	Vector3 vScale = m_pWeaponImage->GetScale();
	m_pWeaponImage->SetTexture(pWeaponTex);
	m_pWeaponImage->SetImageTransPaBlt(Vector2{ 0,0 }, Vector2{ pWeaponTex->GetWidth(), pWeaponTex->GetHeight() }, m_pWeaponImage->GetScale(), EXCEPTION_COLOR_RGB_BLACK);
}