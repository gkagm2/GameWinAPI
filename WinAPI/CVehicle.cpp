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
	m_vDir{0.f ,1.f, 0.f},
	pnt{}
{
}

CVehicle::~CVehicle()
{
}

void CVehicle::Update()
{
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
			UINT iWidth = (UINT)ScaleX();
			UINT iHeight = (UINT)ScaleY();
			UINT iWidth1 = GetTexture()->GetWidth();
			UINT iHeight1 = GetTexture()->GetHeight();
			HDC hTextureDC = GetTexture()->GetDC();

			// 예외처리할 색상 RGB값을 처리하기 위해 BitBlt대신 TransparentBlt을 이용 (library 필요)
			
#pragma region Test code
			
			Vector3 vMiddle = m_vMiddlePos;

			Vector3 dis[3];
			dis[0] = pnt[0] - vMiddle;
			dis[1] = pnt[1] - vMiddle;
			dis[2] = pnt[2] - vMiddle;

			float fDis[3];
			fDis[0] = dis[0].GetDistance();
			fDis[1] = dis[1].GetDistance();
			fDis[2] = dis[2].GetDistance();

			dis[0].Normalized();
			dis[1].Normalized();
			dis[2].Normalized();

			if (InputKeyHold(E_Key::LEFT)) {
				dis[0] = Rotate(dis[0], 120 * DeltaTime);
				dis[1] = Rotate(dis[1], 120 * DeltaTime);
				dis[2] = Rotate(dis[2], 120 * DeltaTime);
			}
			if (InputKeyHold(E_Key::RIGHT)) {
				dis[0] = Rotate(dis[0], -120 * DeltaTime);
				dis[1] = Rotate(dis[1], -120 * DeltaTime);
				dis[2] = Rotate(dis[2], -120 * DeltaTime);
			}
			
			/*if (InputKeyHold(E_Key::RIGHT)) {
				Rotate(dis[0], -60.f);
				Rotate(dis[1], -60.f);
				Rotate(dis[2], -60.f);
			}*/

			pnt[0] = m_vMiddlePos + dis[0] * fDis[0];
			pnt[1] = m_vMiddlePos + dis[1] * fDis[1];
			pnt[2] = m_vMiddlePos + dis[2] * fDis[2];

			POINT rPNT[3];
			rPNT[0].x = pnt[0].x;
			rPNT[0].y = pnt[0].y;
			rPNT[1].x = pnt[1].x;
			rPNT[1].y = pnt[1].y;
			rPNT[2].x = pnt[2].x;
			rPNT[2].y = pnt[2].y;
			
			HBITMAP bitmap{};
			PlgBlt(_hDC, rPNT, GetTexture()->GetDC(), 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), bitmap, 0, 0);
				
#pragma endregion
		}
	}
	
	if (nullptr != GetCollider()) {
		GetCollider()->Render(_hDC);
	}
}