#include "stdafx.h"
#include "CCamera.h"
#include "CCore.h"
#include "CResourceManager.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CTexture.h"

#include "CDebug.h"

CCamera::CCamera() :
	m_vLook{0.f, 0.f, 0.f },
	m_pTargetObject(nullptr),
	m_fSpeed(200.0f),
	m_vDiff{0.f, 0.f, 0.f},
	m_pVeil(nullptr),
	m_eCurEffectState(E_CamEffect::NONE),
	m_fEndEffectTime(5.f),
	m_fCurEffectTime(0.f),
	m_Alpha(255),
	m_fRatio(0.f)
{
}

CCamera::~CCamera()
{
}

void CCamera::Init()
{
	POINT ptResolution = CCore::GetInstance()->GetResolution();
	m_pVeil = CResourceManager::GetInstance()->CreateTexture(STR_TABLE_CameraVeilTexture, (UINT)ptResolution.x, (UINT)ptResolution.y);
}

void CCamera::Update()
{
	Move();
	UpdateEffect();
	Debug->Print(Vector2(80, 80), L"d", m_fCurEffectTime);
	Debug->Print(Vector2(80, 110), L"d", m_fRatio);

	Vector3 resolutionPos = CCore::GetInstance()->GetResolution();
	Vector3 vResolutionCenterPosition = resolutionPos / 2.0f;
	m_vDiff = m_vLook - vResolutionCenterPosition;
}

void CCamera::Render(HDC _hDC)
{
	if (0 == m_Alpha) {
		return;
	}

	UINT iWidth = m_pVeil->GetWidth();
	UINT iHeight = m_pVeil->GetHeight();

	// Texture를 alpha blend와 섞는다.
	BLENDFUNCTION tBlendFunc;
	tBlendFunc.BlendOp = AC_SRC_OVER;
	tBlendFunc.BlendFlags = 0;					// 반드시 0
	tBlendFunc.SourceConstantAlpha = m_Alpha;	// 0 : 투명, 255 불투명
	tBlendFunc.AlphaFormat = 0;					// AC_SRC_ALPHA

	AlphaBlend(_hDC,
		0, 0, (int)iWidth, (int)iHeight,
		m_pVeil->GetDC(),
		0, 0, (int)iWidth, (int)iHeight,
		tBlendFunc);
}

void CCamera::Move()
{
	if (InputKeyHold(E_Key::W))
		m_vLook.y -= m_fSpeed * DeltaTime;
	if (InputKeyHold(E_Key::A))
		m_vLook.x -= m_fSpeed * DeltaTime;
	if (InputKeyHold(E_Key::S))
		m_vLook.y += m_fSpeed * DeltaTime;
	if (InputKeyHold(E_Key::D))
		m_vLook.x += m_fSpeed * DeltaTime;
}

void CCamera::UpdateEffect()
{
	if (E_CamEffect::NONE == m_eCurEffectState)
		return;
	
	m_fCurEffectTime += DeltaTime;
	if (m_fCurEffectTime > m_fEndEffectTime) {
		m_fCurEffectTime = m_fEndEffectTime;
		m_eCurEffectState = E_CamEffect::NONE;
	}

	m_fRatio = m_fCurEffectTime / m_fEndEffectTime;
	if (E_CamEffect::FADE_IN == m_eCurEffectState) {
		m_fRatio = 1.f - m_fRatio;
		m_Alpha = (BYTE)(255.f * m_fRatio);
	}
}

void CCamera::SetEffect(E_CamEffect _eCamEffect, float _fTime)
{
	m_eCurEffectState = _eCamEffect;
	m_fEndEffectTime = _fTime;
	m_fCurEffectTime = 0.f;
}