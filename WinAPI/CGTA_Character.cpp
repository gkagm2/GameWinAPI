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

CGTA_Character::CGTA_Character(E_GroupType _eGroupType) :
	CObject(_eGroupType)
{
}

CGTA_Character::~CGTA_Character()
{
}
	
void CGTA_Character::Init()
{
	CObject::Init();
}

void CGTA_Character::PrevUpdate()
{
}

void CGTA_Character::Update()
{
}

void CGTA_Character::LateUpdate()
{
}

void CGTA_Character::Render(HDC _hDC)
{
	if (false == IsRender())
		return;

	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	if (nullptr == GetTexture()) {

	}
	else {
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