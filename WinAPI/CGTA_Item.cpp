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
	m_eItemType(E_ItemType::NONE)
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

	// Set Collider
	CColliderRect* pCollider = new CColliderRect(this);
	pCollider->SetScale(Vector3(10.f, 10.f, 10.f));
	SetCollider(pCollider);


	// Item Type setting

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
			(int)vRenderPos.x, (int)vRenderPos.y, (int)GetScale().x, (int)GetScale().y, 
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