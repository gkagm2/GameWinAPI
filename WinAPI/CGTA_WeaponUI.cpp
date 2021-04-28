#include "stdafx.h"
#include "CGTA_WeaponUI.h"
#include "CCore.h"
#include "CResource.h"
#include "CResourceManager.h"
#include "CTexture.h"

CGTA_WeaponUI::CGTA_WeaponUI(E_GroupType _eGroupType) :
	CUI(_eGroupType)
{
}

CGTA_WeaponUI::~CGTA_WeaponUI()
{
}

void CGTA_WeaponUI::Init()
{
	POINT rect = CCore::GetInstance()->GetResolution();
	SetPosition(rect.x - 300, rect.y - 300);
}

void CGTA_WeaponUI::Update()
{
	CUI::Update();
}

void CGTA_WeaponUI::Render(HDC _hDC)
{
	if (nullptr == GetTexture())
		return;

	Vector3 vFinalPos = GetFinalPosition();
	Vector3 vScale = GetScale();

	wchar_t buff[255];
	TextOut(_hDC, vFinalPos.x, vFinalPos.y + 150, m_tWeaponInfo.strName.c_str(), wcslen(m_tWeaponInfo.strName.c_str()));

	swprintf(buff, 255, L"Ammo : %d", m_tWeaponInfo.iBulletCnt);
	TextOut(_hDC, vFinalPos.x, vFinalPos.y + 180, buff, wcslen(buff));

	TransparentBlt(_hDC, (int)vFinalPos.x, (int)vFinalPos.y, (int)GetScale().x, (int)GetScale().y, GetTexture()->GetDC(), 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), 0);
	
	CUI::Render(_hDC);
}

void CGTA_WeaponUI::ChangeWeaponUI(const TWeaponInfo& _tWeaponInfo)
{
	m_tWeaponInfo = _tWeaponInfo;
	if (m_tWeaponInfo.strName.compare(STR_NAME_Fist) == 0) {
		SetTexture(nullptr);
		return;
	}
	wstring strPath = STR_FILE_PATH_GTA_Texture + m_tWeaponInfo.strName + L".bmp";
	CTexture* pWeaponTex = CResourceManager::GetInstance()->FindTexture(strPath);
	if (nullptr == pWeaponTex) {
		pWeaponTex = CResourceManager::GetInstance()->LoadTexture(strPath, strPath);
		assert(pWeaponTex);
	}
	SetTexture(pWeaponTex);
}