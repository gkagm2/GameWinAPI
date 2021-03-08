#include "stdafx.h"
#include "CObject.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CCollider.h"
#include "CSceneManager.h"
#include "CScene.h"

CObject::CObject(E_GroupType e_GroupType = E_GroupType::DEFAULT) :
	m_vPosition{ 0, 0, 0 },
	m_vScale{ 100.0f, 100.0f, 100.0f },
	m_pTexture(nullptr),
	m_pCollider(nullptr),
	m_eGroupType(e_GroupType),
	m_strName(STR_OBJECT_DEFAULT_NAME),
	m_bIsDead(false),
	m_bIsRender(true)
{
}

CObject::~CObject()
{
	if (nullptr != m_pCollider) // 존재하면
		delete m_pCollider;
}

void CObject::LateUpdate()
{
	if (nullptr != m_pCollider)
		m_pCollider->LateUpdate();
}

void CObject::Render(HDC _hDC)
{
	if (false == m_bIsRender)
		return;

	Vector3 vRenderPosition = CCamera::GetInstance()->GetRenderPosition(m_vPosition);

	if (nullptr == m_pTexture) {
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
		UINT iWidth = (UINT)ScaleX();
		UINT iHeight = (UINT)ScaleY();
		UINT iWidth1 = m_pTexture->GetWidth();
		UINT iHeight1 = m_pTexture->GetHeight();
		HDC hTextureDC = m_pTexture->GetDC();

		// 예외처리할 색상 RGB값을 처리하기 위해 BitBlt대신 TransparentBlt을 이용 (library 필요)
		TransparentBlt(
			_hDC,
			(int)(vRenderPosition.x - iWidth / 2), (int)(vRenderPosition.y - iHeight / 2),
			iWidth, iHeight,
			hTextureDC,
			0, 0,
			iWidth1, iHeight1,
			(COLORREF)EXCEPTION_COLOR_RGB); // 제거 할 색상

		/*BitBlt(
			_hDC,
			(int)(m_vPos.x - iWidth / 2),
			(int)(m_vPos.y - iHeight / 2),
			iWidth, iHeight,
			hTextureDC,
			0, 0,
			SRCCOPY);*/

		
		// Print Position
		if (GetAsyncKeyState(0x30) & 0x8000) {
			wchar_t pStrPosition[100] = { 0, };
			swprintf(pStrPosition, 100, L"(%.2f, %.2f)", vRenderPosition.x, vRenderPosition.y);


			SetTextAlign(_hDC, TA_CENTER);
			TextOut(_hDC, (int)vRenderPosition.x, (int)(vRenderPosition.y - iWidth / 2 - 30), pStrPosition, (int)wcslen(pStrPosition));
			SetTextAlign(_hDC, TA_LEFT | TA_TOP);
		}
	}

	// ColliderRendering
	if (nullptr != m_pCollider) {
		m_pCollider->Render(_hDC);
	}
	
	/*
	float f;
	int y;
	CKeyManager* keyMrg = CKeyManager::GetInstance();
	if (keyMrg->GetKeyState(E_Key::A) == E_KeyState::HOLD) {
		pathX -= 1 * DeltaTime * 100.0f;
		pathMaxX -= 1 * DeltaTime * 100.0f;
	}
	if (keyMrg->GetKeyState(E_Key::D) == E_KeyState::HOLD) {
		pathMaxX += 1 * DeltaTime * 100.0f;
		pathX += 1 * DeltaTime * 100.0f;

	}

	for (f= pathX; f <= pathMaxX; ++f) {
		y = (int)(sinf(f * 3.14f / 180) * 200);
		SetPixel(_hDC, (int)f, y + CCore::GetInstance()->GetResolution().y / 2, RGB(0, 0, 0));
	}*/
}

Vector3 CObject::GetMax()
{
	Vector3 maxVec = m_vPosition;
	if (nullptr != m_pTexture) {
		maxVec.x += m_pTexture->GetWidth() / 2.0f;
		maxVec.y += m_pTexture->GetHeight() / 2.0f;
	}
	else
		maxVec += m_vScale / 2.0f;
	return maxVec;
}

void CObject::SetTexture(CTexture* _pTexture)
{
	m_pTexture = _pTexture;
	m_vScale.x = (float)m_pTexture->GetWidth();
	m_vScale.y = (float)m_pTexture->GetHeight();
}

Vector3 CObject::GetMin()
{
	Vector3 minVec = m_vPosition;
	if (nullptr != m_pTexture) {
		minVec.x -= m_pTexture->GetWidth() / 2.0f;
		minVec.y -= m_pTexture->GetHeight() / 2.0f;
	}
	else
		minVec -= m_vScale / 2.0f;
	return minVec;
}
