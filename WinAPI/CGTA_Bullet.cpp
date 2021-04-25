#include "stdafx.h"
#include "CGTA_Bullet.h"
#include "CColliderRect.h"
#include "CTimeManager.h"
#include "CTexture.h"
#include "CResource.h"
#include "CResourceManager.h"
#include "CCamera2D.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CDebug.h"
#include "CTile.h"
#include "CGTA_Citizen.h"
#include "CGTA_Player.h"
#include "CGTA_Vehicle.h"


CGTA_Bullet::CGTA_Bullet(E_GroupType e_GroupType) :
	CObject(e_GroupType),
	m_fLifeCoolTime(0.f),
	m_fLifeMaxCoolTime(3.f),
	m_fSpeed(1000.f),
	m_fDamage(1.f)
{
}

CGTA_Bullet::CGTA_Bullet(const CGTA_Bullet& _origin) :
	CObject(_origin),
	m_fLifeCoolTime(0.f),
	m_fLifeMaxCoolTime(_origin.m_fLifeMaxCoolTime),
	m_fSpeed(_origin.m_fSpeed),
	m_fDamage(_origin.m_fDamage)
{
}

CGTA_Bullet::~CGTA_Bullet()
{
}

void CGTA_Bullet::Init()
{
	// 텍스쳐 설정
	CTexture* pTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_PATH_gta_Bullet);
	if (nullptr == pTexture) {
		pTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_PATH_gta_Bullet, STR_FILE_PATH_gta_Bullet);
	}
	SetTexture(pTexture);

	// 충돌 설정
	CColliderRect* pCollider = new CColliderRect(this);
	pCollider->SetScale(Vector3(10.f, 10.f, 0.f));
	pCollider->SetTrigger(true);

	// 크기 설정
	SetScale(Vector3(30.f, 30.f, 0.f));

	CObject::Init();
}

void CGTA_Bullet::Update()
{
	m_fLifeCoolTime += DeltaTime;
	if (m_fLifeCoolTime >= m_fLifeMaxCoolTime)
		DestroyObject(this);

	Vector3 vHeadDir = GetUpVector();
	SetPosition(GetPosition() + vHeadDir * DeltaTime * m_fSpeed);
}

void CGTA_Bullet::Render(HDC _hDC)
{
	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	BitBlt(
		_hDC,
		(int)(vRenderPosition.x - GetTexture()->GetWidth() * 0.5f),
		(int)(vRenderPosition.y - GetTexture()->GetHeight() * 0.5f),
		GetTexture()->GetWidth(),
		GetTexture()->GetHeight(),
		GetTexture()->GetDC(),
		0,
		0,
		SRCCOPY);

		// ColliderRendering
	if (nullptr != GetCollider()) {
		if (GetCollider()->IsRender())
			GetCollider()->Render(_hDC);
	}
}

void CGTA_Bullet::OnCollisionEnter(CObject* _pOther)
{
	auto pTile = dynamic_cast<CTile*>(_pOther);
	if (pTile) {
		if (E_TileType::Wall == pTile->GetTileType())
			DestroyObject(this);
		return;
	}
	auto pCharacter = dynamic_cast<CGTA_Character*>(_pOther);
	if (pCharacter) {
		DestroyObject(this);
		return;
	}

	auto pVehicle = dynamic_cast<CGTA_Vehicle*>(_pOther);
	if (pVehicle) {
		DestroyObject(this);
		return;
	}
}

void CGTA_Bullet::OnCollisionStay(CObject* _pOther)
{
}

void CGTA_Bullet::OnCollisionExit(CObject* _pOther)
{
}