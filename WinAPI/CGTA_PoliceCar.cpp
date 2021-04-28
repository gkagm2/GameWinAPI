#include "stdafx.h"
#include "CGTA_PoliceCar.h"
#include "CResourceManager.h"
#include "CResource.h"
#include "CAnimator.h"
#include "CTexture.h"
#include "CColliderRect.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CCamera.h"
#include "CSceneManager.h"
#include "CScene.h"

// Test
#include "CCore.h"
#include "CDebug.h"
#include "CTile.h"

CGTA_PoliceCar::CGTA_PoliceCar(E_GroupType e_GroupType) :
	CGTA_Vehicle(e_GroupType)
{
}

CGTA_PoliceCar::CGTA_PoliceCar(const CGTA_PoliceCar& _origin) :
	CGTA_Vehicle(_origin)
{
}

CGTA_PoliceCar::~CGTA_PoliceCar()
{
}

void CGTA_PoliceCar::Init()
{
	CTexture* pVehicleTexAnim = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_Vehicle_car_Cops1Anim);
	if (nullptr == pVehicleTexAnim)
		pVehicleTexAnim = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_Vehicle_car_Cops1Anim, STR_FILE_PATH_Vehicle_car_Cops1Anim);
	SetTexture(pVehicleTexAnim);

	/*CAnimator* pAnimator = new CAnimator(this);
	float height = pVehicleTexAnim->GetHeight();
	float width = pVehicleTexAnim->GetWidth();
	float texHeight = (pVehicleTexAnim->GetHeight() - 10) * 0.5f;
	float texWidth = (pVehicleTexAnim->GetWidth() - 90) / 10.2f;
	pAnimator->CreateAnimation(L"idle", pVehicleTexAnim, Vector2(0, 0), Vector2(texWidth, texHeight), 1, 1);
	pAnimator->PlayAnimation(L"idle", E_AnimationPlayType::LOOP);*/

	SetPosition(0, 0, 0);
	SetScale(Vector3(GetScale().x * 0.5f, GetScale().y * 0.5f, 0.0f));

	CColliderRect* pColRect = new CColliderRect(this);
	pColRect->SetScale(Vector3(GetScale().x , GetScale().y, 0.f));
	__super::Init();
}

void CGTA_PoliceCar::PrevUpdate()
{
	__super::PrevUpdate();
}

void CGTA_PoliceCar::Update()
{
	__super::Update();

	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());
}

void CGTA_PoliceCar::LateUpdate()
{
	__super::LateUpdate();
}

void CGTA_PoliceCar::Render(HDC _hDC)
{
	__super::Render(_hDC);
}

void CGTA_PoliceCar::OnCollisionEnter(CObject* _pOther)
{
	CGTA_Vehicle::OnCollisionEnter(_pOther);
}

void CGTA_PoliceCar::OnCollisionStay(CObject* _pOther)
{
	CColliderRect* pColRect = dynamic_cast<CColliderRect*>(_pOther->GetCollider());
	if (nullptr != pColRect) {
		CTile* pTile = dynamic_cast<CTile*>(_pOther);
		
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // Green color
		HPEN hOldPen = (HPEN)SelectObject(CCore::GetInstance()->GetDC(), hPen);

		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(CCore::GetInstance()->GetDC(), myBrush);
		Vector3 vOther = MainCamera->GetRenderPosition(_pOther->GetPosition() + pColRect->GetOffsetPosition());
		Vector3 vThis = MainCamera->GetRenderPosition(GetPosition());
		MoveToEx(CCore::GetInstance()->GetDC(), (int)vOther.x, (int)vOther.y, nullptr);
		LineTo(CCore::GetInstance()->GetDC(), (int)vThis.x, (int)vThis.y);

		SelectObject(CCore::GetInstance()->GetDC(), oldBrush);
		SelectObject(CCore::GetInstance()->GetDC(), hOldPen);
		DeleteObject(myBrush);
		DeleteObject(hPen);
	}
	CGTA_Vehicle::OnCollisionStay(_pOther);
}

void CGTA_PoliceCar::OnCollisionExit(CObject* _pOther)
{
}