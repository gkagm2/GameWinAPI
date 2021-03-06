#include "stdafx.h"
#include "CCamera2D.h"
#include "CTexture.h"
#include "CCore.h"
#include "CResourceManager.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CCore.h"
#include "CRigidbody2D.h"
#include "CGTA_Player.h"
#include "CGTA_Vehicle.h"
#include "CDebug.h"
CCamera2D::CCamera2D(E_GroupType _eGroupType) :
	CCamera(_eGroupType),
	m_pVeil(nullptr),
	m_Alpha(255),
	m_fRatio(0.f)
{
}

CCamera2D::~CCamera2D()
{
}

void CCamera2D::Init()
{
	POINT ptResolution = CCore::GetInstance()->GetResolution();
	m_pVeil = CResourceManager::GetInstance()->FindTexture(STR_TABLE_CameraVeilTexture);
	if(nullptr == m_pVeil)
		m_pVeil = CResourceManager::GetInstance()->CreateTexture(STR_TABLE_CameraVeilTexture, (UINT)ptResolution.x, (UINT)ptResolution.y);
}

void CCamera2D::Update()
{
	if (dynamic_cast<CGTA_Player*>(m_pTargetObject))
		Move();
	else if (dynamic_cast<CGTA_Vehicle*>(m_pTargetObject))
		MoveVehicle();
	
	UpdateEffect();
	__super::Update();
}

void CCamera2D::Render(HDC _hDC)
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

void CCamera2D::UpdateEffect()
{
	if (m_queEffect.empty())
		return;

	tCamEffect* ptCamEffect = &m_queEffect.front();
	if (E_CamEffect::NONE == ptCamEffect->eEffect) {
		m_queEffect.pop();
		return;
	}

	ptCamEffect->fCurTime += DeltaTime;
	if (ptCamEffect->fCurTime > ptCamEffect->fEndTime) {
		ptCamEffect->fCurTime = ptCamEffect->fEndTime;
		ptCamEffect->eEffect = E_CamEffect::NONE;
	}

	m_fRatio = ptCamEffect->fCurTime / ptCamEffect->fEndTime;
	if (E_CamEffect::FADE_IN == ptCamEffect->eEffect) {
		m_fRatio = 1.f - m_fRatio;
		m_Alpha = (BYTE)(255.f * m_fRatio);
	}
	else if (E_CamEffect::FADE_OUT == ptCamEffect->eEffect) {
		m_Alpha = (BYTE)(255.f * m_fRatio);
	}
}

void CCamera2D::Move()
{
	Vector3 vPos = m_vLook;
	Vector3 vTargetPos = m_pTargetObject->GetPosition();
	vTargetPos = CMyMath::Lerp(vPos, vTargetPos, 5.f * DeltaTime);
	m_vLook = vTargetPos;
}

void CCamera2D::MoveVehicle()
{
	// 속도에 따라 달라진다.
	Vector3 vPos = m_vLook;
	float fSpeed = m_pTargetObject->GetRigidbody()->GetSpeed();
	float fMaxPower = 20.f;
	fSpeed = min(fMaxPower, fSpeed);

	Vector3 vTargetPos = m_pTargetObject->GetPosition();
	if (fSpeed > 3) {
		CGTA_Vehicle* pVehicle = (CGTA_Vehicle*)m_pTargetObject;
		if(false == pVehicle->IsReverse())
			vTargetPos += (-m_pTargetObject->GetUpVector()) * fSpeed * 35.f;
	}
	vTargetPos = CMyMath::Lerp(vPos, vTargetPos, 6.f * DeltaTime);
	m_vLook = vTargetPos;
}

void CCamera2D::AddEffect(E_CamEffect _eCamEffect, float _fEndTime)
{
	m_queEffect.push(tCamEffect{ _eCamEffect, _fEndTime, 0.f });
}
