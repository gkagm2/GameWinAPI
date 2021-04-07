#include "stdafx.h"

#include "CScene.h"
#include "CSceneManager.h"
#include "CResourceManager.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CCore.h"
#include "CVehicle.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"

CVehicle::CVehicle(E_GroupType _eGroupType) :
	CObject(_eGroupType = E_GroupType::VEHICLE),
	m_vPrevHeadDir{0, 1, 0}
{
}

CVehicle::~CVehicle()
{
}

void CVehicle::Init()
{
	InitRectPoint();
}

void CVehicle::PrevUpdate()
{
	__super::PrevUpdate();
}

void CVehicle::Update()
{
	// This is Get direction 
	//RotateRP(0);

	Vector3 vHeadDir = m_vPrevHeadDir;
	vHeadDir.Normalized();
	//RotateInfo().Update();
	if (InputKeyHold(E_Key::LEFT)) {
		RotateRP(-180);
		vHeadDir = Rotate(vHeadDir, -180 * DeltaTime);
	}
	if (InputKeyHold(E_Key::RIGHT)) {
		RotateRP(180);
		vHeadDir = Rotate(vHeadDir, 180 * DeltaTime);
	}
	m_vPrevHeadDir = vHeadDir;

	if (InputKeyHold(E_Key::UP)) {
		SetPosition(GetPosition().x + vHeadDir.x * 300 * DeltaTime, GetPosition().y + vHeadDir.y * 300 * DeltaTime);
	}
	if (InputKeyHold(E_Key::DOWN)) {
		SetPosition(GetPosition().x - vHeadDir.x * 300 * DeltaTime, GetPosition().y - vHeadDir.y * 300 * DeltaTime);
	}
}

void CVehicle::LateUpdate()
{
	__super::LateUpdate();
}

void CVehicle::Render(HDC _hDC)
{
	if (false == IsRender())
		return;

	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	if (nullptr == GetTexture()) {
		Rectangle(_hDC,
			(int)(vRenderPosition.x - ScaleX() / 2),
			(int)(vRenderPosition.y - ScaleY() / 2),
			(int)(vRenderPosition.x + ScaleX() / 2),
			(int)(vRenderPosition.y + ScaleY() / 2));


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
			//// ����ó���� ���� RGB���� ó���ϱ� ���� BitBlt��� TransparentBlt�� �̿� (library �ʿ�)
			//TransparentBlt(
			//	_hDC,
			//	(int)(vRenderPosition.x - iWidth / 2), (int)(vRenderPosition.y - iHeight / 2),
			//	iWidth, iHeight,
			//	hTextureDC,
			//	0, 0,
			//	iWidth1, iHeight1,
			//	(COLORREF)EXCEPTION_COLOR_RGB_BLACK); // ���� �� ����
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