#include "stdafx.h"
#include "CGTA_Character.h"
#include "CResourceManager.h"
#include "CGTA_Player.h"
#include "CColliderRect.h"
#include "CTexture.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CKeyManager.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CCamera.h"
#include "CObject.h"
#include "CGTA_PunchDetector.h"

CGTA_Character::CGTA_Character(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_vNozzlePos(GetUpVector() * 5.f),
	m_pVehicle(nullptr),
	m_pPunchDetector(nullptr)
{
}
CGTA_Character::CGTA_Character(const CGTA_Character& _origin) :
	CObject(_origin),
	m_pVehicle(nullptr),
	m_vNozzlePos(GetUpVector() * 5.f),
	m_pPunchDetector(nullptr)
{
	m_pPunchDetector = _origin.m_pPunchDetector->Clone();
}

CGTA_Character::~CGTA_Character()
{
	if (nullptr != m_pPunchDetector)
		delete m_pPunchDetector;
}
	
void CGTA_Character::Init()
{
	m_pPunchDetector = new CGTA_PunchDetector(E_GroupType::PROJECTILE);
	CreateObject(m_pPunchDetector);
	CObject::Init();
}

void CGTA_Character::PrevUpdate()
{
	__super::PrevUpdate();
}

void CGTA_Character::Update()
{
}

void CGTA_Character::LateUpdate()
{
	__super::LateUpdate();
}

void CGTA_Character::Render(HDC _hDC)
{
	if (false == IsRender())
		return;

	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	if (GetTexture()) {
		if (nullptr != GetAnimator())
			GetAnimator()->Render(_hDC);
		else {
			POINT rPNT[3];

			Vector3 v1 = GetRectPoint(0);
			Vector3 v2 = GetRectPoint(1);
			Vector3 v3 = GetRectPoint(2);
			rPNT[0].x = (int)(vRenderPosition.x + GetRectPoint(0).x);
			rPNT[0].y = (int)(vRenderPosition.y + GetRectPoint(0).y);
			rPNT[1].x = (int)(vRenderPosition.x + GetRectPoint(1).x);
			rPNT[1].y = (int)(vRenderPosition.y + GetRectPoint(1).y);
			rPNT[2].x = (int)(vRenderPosition.x + GetRectPoint(2).x);
			rPNT[2].y = (int)(vRenderPosition.y + GetRectPoint(2).y);

			HBITMAP bitmap{};
			PlgBlt(_hDC, rPNT, GetTexture()->GetDC(), 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), bitmap, 8, 5);
		}
	}

	if (nullptr != GetCollider()) {
		GetCollider()->Render(_hDC);
	}
	//__super::Render(_hDC);
}

void CGTA_Character::OnCollisionEnter(CObject* _pOther)
{
}

void CGTA_Character::OnCollisionStay(CObject* _pOther)
{
}

void CGTA_Character::OnCollisionExit(CObject* _pOther)
{
}