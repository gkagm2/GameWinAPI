#include "stdafx.h"

#include "CScene.h"
#include "CSceneManager.h"
#include "CResourceManager.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CCore.h"
#include "CGTA_Vehicle.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidbody2D.h"

CGTA_Vehicle::CGTA_Vehicle(E_GroupType _eGroupType) :
	CObject(_eGroupType = E_GroupType::VEHICLE)
{
}

CGTA_Vehicle::CGTA_Vehicle(const CGTA_Vehicle& _origin) :
	CObject(_origin)
{
}

CGTA_Vehicle::~CGTA_Vehicle()
{
}

void CGTA_Vehicle::Init()
{
	CRigidbody2D* pRigidbody = new CRigidbody2D(this);
	CObject::Init();
}

void CGTA_Vehicle::PrevUpdate()
{
	CObject::PrevUpdate();
}

void CGTA_Vehicle::Update()
{
	// This is Get direction 
	//RotateRP(0);

	Vector3 vHeadDir = GetUpVector();
	//RotateInfo().Update();
	if (InputKeyHold(E_Key::LEFT)) {
		RotateRP(-180 * DeltaTime);
	}
	if (InputKeyHold(E_Key::RIGHT)) {
		RotateRP(180 * DeltaTime);
	}

	if (InputKeyHold(E_Key::UP)) {
		SetPosition(GetPosition().x - GetUpVector().x * 300 * DeltaTime, GetPosition().y - GetUpVector().y * 300 * DeltaTime);
	}
	if (InputKeyHold(E_Key::DOWN)) {
		SetPosition(GetPosition().x + GetUpVector().x * 300 * DeltaTime, GetPosition().y + GetUpVector().y * 300 * DeltaTime);
	}
}

void CGTA_Vehicle::LateUpdate()
{
	__super::LateUpdate();
}

void CGTA_Vehicle::Render(HDC _hDC)
{
	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	if (nullptr == GetTexture()) {
		Rectangle(_hDC,
			(int)(vRenderPosition.x - ScaleX() * 0.5f),
			(int)(vRenderPosition.y - ScaleY() * 0.5f),
			(int)(vRenderPosition.x + ScaleX() * 0.5f),
			(int)(vRenderPosition.y + ScaleY() * 0.5f));


		if (GetAsyncKeyState(0x30) & 0x8000) {
			// Print Position
			wchar_t pStrPosition[100] = { 0, };
			swprintf(pStrPosition, 100, L"(%.2f, %.2f)", vRenderPosition.x, vRenderPosition.y);

			SetTextAlign(_hDC, TA_CENTER);
			TextOut(_hDC, (int)vRenderPosition.x, (int)(vRenderPosition.y), pStrPosition, (int)wcslen(pStrPosition));
			SetTextAlign(_hDC, TA_LEFT | TA_TOP);
		}
	}
	else {

		if (nullptr != GetAnimator())
			GetAnimator()->Render(_hDC);
		else {
			//UINT iWidth = (UINT)ScaleX();
			//UINT iHeight = (UINT)ScaleY();
			//UINT iWidth1 = GetTexture()->GetWidth();
			//UINT iHeight1 = GetTexture()->GetHeight();
			//HDC hTextureDC = GetTexture()->GetDC();
			//// 예외처리할 색상 RGB값을 처리하기 위해 BitBlt대신 TransparentBlt을 이용 (library 필요)
			//TransparentBlt(
			//	_hDC,
			//	(int)(vRenderPosition.x - iWidth * 0.5f), (int)(vRenderPosition.y - iHeight * 0.5f),
			//	iWidth, iHeight,
			//	hTextureDC,
			//	0, 0,
			//	iWidth1, iHeight1,
			//	(COLORREF)EXCEPTION_COLOR_RGB_BLACK); // 제거 할 색상
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
			PlgBlt(_hDC, rPNT, GetTexture()->GetDC(), 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), bitmap, 8,5);
		}
	}
	
	if (nullptr != GetCollider()) {
		GetCollider()->Render(_hDC);
	}
}