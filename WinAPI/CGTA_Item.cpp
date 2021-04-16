#include "stdafx.h"
#include "CGTA_Item.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CResource.h"
#include "CColliderRect.h"
#include "CCamera.h"
#include "CCamera2D.h"
#include "CScene.h"
#include "CSceneManager.h"

CGTA_Item::CGTA_Item(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_vLT{},
	m_eItemType(E_ItemType::PISTOL)
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

	// Set Collider
	CColliderRect* pCollider = new CColliderRect(this);
	SetCollider(pCollider);
	pCollider->SetScale(Vector3(GetScale().x * 0.5, GetScale().y * 0.5f, 0.f));

	SetPosition(0, 0, 0);

	// Item Type setting
	CObject::Init();
}

void CGTA_Item::PrevUpdate()
{
	CObject::PrevUpdate();
}

void CGTA_Item::Update()
{
}

void CGTA_Item::LateUpdate()
{
	CObject::LateUpdate();
}

void CGTA_Item::Render(HDC _hDC)
{
	if(nullptr == GetTexture()) {
		assert(nullptr && L"Texture°¡ ¾øÀ½");
		return;
	}

	if (GetTexture()) {
		CTexture* pTexture = GetTexture();
		const Vector3& vRenderPos = MainCamera->GetRenderPosition(GetPosition());
		TransparentBlt(_hDC, 
			(int)vRenderPos.x - GetScale().x * 0.5f, (int)vRenderPos.y - GetScale().y * 0.5f, (int)GetScale().x, (int)GetScale().y, 
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
}

void CGTA_Item::Save(FILE* _pFile)
{
	fwrite(&m_vLT, sizeof(Vector2), 1, _pFile);
	fwrite(&m_eItemType, sizeof(E_ItemType), 1, _pFile);
	CObject::Save(_pFile);
}

void CGTA_Item::Load(FILE* _pFile)
{
	fread(&m_vLT, sizeof(Vector2), 1, _pFile);
	fread(&m_eItemType, sizeof(E_ItemType), 1, _pFile);
	CObject::Load(_pFile);
}