#include "stdafx.h"
#include "CGTA_Item.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CResource.h"
#include "CColliderRect.h"
#include "CCollider.h"
#include "CCamera.h"
#include "CCamera2D.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CGTA_Player.h"
#include "CTimeManager.h"
#include "CDebug.h"

CGTA_Item::CGTA_Item(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_vLT{},
	m_eItemType(E_ItemType::WEAPON),
	m_eWeaponType{E_WeaponType::FIST},
	m_tWeaponInfo{},
	m_fTime(0.f),
	m_fRespawnCoolTime(0.f),
	m_fMaxRespawnCoolTime(10.f),
	m_eItemState(E_ItemState::ON)
{
}

CGTA_Item::~CGTA_Item()
{
}

void CGTA_Item::Init()
{
	// Set Texture
	CTexture* pTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_PATH_gta_item);
	if (nullptr == pTexture) {
		pTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_PATH_gta_item, STR_FILE_PATH_gta_item);
	}
	SetTexture(pTexture);
	SetScale(60.f, 60.f, 0.f);
	m_vFixedScale = GetScale();

	// Set Collider
	CColliderRect* pCollider = new CColliderRect(this);
	SetCollider(pCollider);
	pCollider->SetScale(Vector3(GetScale().x * 0.5f, GetScale().y * 0.5f, 0.f));

	SetPosition(0, 0, 0);
	
	// Item Type setting
	CObject::Init();
}

void CGTA_Item::PrevUpdate()
{
	if (E_ItemState::OFF == m_eItemState)
		return;
	CObject::PrevUpdate();
}

void CGTA_Item::Update()
{
	if (E_ItemState::ON == m_eItemState) {
		m_fTime += DeltaTime * 2.f;
		SetScale(m_vFixedScale.x + sinf(m_fTime) * 5.f, m_vFixedScale.y + sinf(m_fTime) * 5.f, m_vFixedScale.z);
	}
	else if(E_ItemState::OFF == m_eItemState) {
		m_fRespawnCoolTime += DeltaTime;
		if(m_fRespawnCoolTime >= m_fMaxRespawnCoolTime) {
			SetItemState(E_ItemState::ON);
		}
	}
}

void CGTA_Item::LateUpdate()
{
	if (E_ItemState::OFF == m_eItemState)
		return;
	CObject::LateUpdate();
}

void CGTA_Item::Render(HDC _hDC)
{
	if (E_ItemState::OFF == m_eItemState)
		return;

	if(nullptr == GetTexture()) {
		assert(nullptr && L"Texture가 없음");
		return;
	}

	if (GetTexture()) {
		CTexture* pTexture = GetTexture();
		const Vector3& vRenderPos = MainCamera->GetRenderPosition(GetPosition());
		TransparentBlt(_hDC, 
			(int)(vRenderPos.x - GetScale().x * 0.5f), (int)(vRenderPos.y - GetScale().y * 0.5f), (int)GetScale().x, (int)GetScale().y, 
			GetTexture()->GetDC(), 
			(int)m_vLT.x, (int)m_vLT.y, TILE_SIZE_ITEM, TILE_SIZE_ITEM,
			EXCEPTION_COLOR_RGB_BLACK);
	}

	// ColliderRendering
	if (nullptr != GetCollider())
		GetCollider()->Render(_hDC);
}

void CGTA_Item::OnCollisionEnter(CObject* _pOther)
{
	if (m_eItemState == E_ItemState::OFF)
		return;

	CGTA_Player* pPlayer = dynamic_cast<CGTA_Player*>(_pOther);
	if (pPlayer) {
		// 무기 아이템이면
		if (E_ItemType::WEAPON == m_eItemType) {
			pPlayer->GetItem(this);
			SetItemState(E_ItemState::OFF);
		}
	}
}

void CGTA_Item::Save(FILE* _pFile)
{
	fwrite(&m_vLT, sizeof(Vector2), 1, _pFile);
	fwrite(&m_eItemType, sizeof(E_ItemType), 1, _pFile);
	fwrite(&m_eWeaponType, sizeof(E_WeaponType), 1, _pFile);
	m_tWeaponInfo.Save(_pFile);

	CObject::Save(_pFile);
}

void CGTA_Item::Load(FILE* _pFile)
{
	fread(&m_vLT, sizeof(Vector2), 1, _pFile);
	fread(&m_eItemType, sizeof(E_ItemType), 1, _pFile);
	fread(&m_eWeaponType, sizeof(E_WeaponType), 1, _pFile);
	m_tWeaponInfo.Load(_pFile);
	CObject::Load(_pFile);
}

void CGTA_Item::SetWeaponInfo(const TWeaponInfo& _tWeaponInfo)
{
	m_tWeaponInfo = _tWeaponInfo;
}

void CGTA_Item::SetItemState(E_ItemState _eItemState)
{
	if (E_ItemState::ON == _eItemState) {
		m_fTime = 0.f;
		GetCollider()->SetActive(true);
	}
		
	else if (E_ItemState::OFF == _eItemState) {
		m_fRespawnCoolTime = 0.f;
		GetCollider()->SetActive(false);
	}
	m_eItemState = _eItemState;
}
