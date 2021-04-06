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

#include "CDebug.h"
CGTA_PoliceCar::CGTA_PoliceCar(E_GroupType e_GroupType) :
	CVehicle(e_GroupType)
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
	float texHeight = (pVehicleTexAnim->GetHeight() - 10) / 2.f;
	float texWidth = (pVehicleTexAnim->GetWidth() - 90) / 10.2f;
	pAnimator->CreateAnimation(L"idle", pVehicleTexAnim, Vector2(0, 0), Vector2(texWidth, texHeight), 1, 1);
	pAnimator->PlayAnimation(L"idle", E_AnimationPlayType::LOOP);*/

	SetPosition(0, 0, 0);
	SetScale(Vector3(GetScale().x, GetScale().y, 0.0f));

	CColliderRect* pColRect = new CColliderRect(this);
	pColRect->SetScale(Vector3(GetScale().x / 2.f, GetScale().y /2.f, 0.f));
	__super::Init();
}

void CGTA_PoliceCar::PrevUpdate()
{
	__super::PrevUpdate();
}

void CGTA_PoliceCar::Update()
{
	if (InputKeyHold(E_Key::UP)) {
		SetPosition( GetPosition().x, GetPosition().y - DeltaTime * 30.f);
	}
	if (InputKeyHold(E_Key::DOWN)) {
		SetPosition(GetPosition().x, GetPosition().y + DeltaTime * 30.f);
	}
	__super::Update();

	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	Debug->Print(vRenderPosition, L"dd", GetPosition().x, GetPosition().y);
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
}

void CGTA_PoliceCar::OnCollisionStay(CObject* _pOther)
{
}

void CGTA_PoliceCar::OnCollisionExit(CObject* _pOther)
{
}