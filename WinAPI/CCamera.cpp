#include "stdafx.h"
#include "CCamera.h"
#include "CCore.h"
#include "CDebug.h"
#include "CObject.h"
#include "CSceneManager.h"
#include "CScene.h"

CCamera::CCamera(E_GroupType _eGroupType = E_GroupType::MAIN_CAMERA) :
	CObject(_eGroupType),
	m_vLook{0.f, 0.f, 0.f },
	m_pTargetObject(nullptr),
	m_fSpeed(200.0f),
	m_vDiff{0.f, 0.f, 0.f}
{
}

CCamera::~CCamera()
{
	CSceneManager::GetInstance()->GetCurScene()->SetMainCamera(nullptr);
}

void CCamera::Init()
{
}

void CCamera::Update()
{
	// 카메라 상속 시 반드시 실행
	Vector3 resolutionPos = CCore::GetInstance()->GetResolution();
	Vector3 vResolutionCenterPosition = resolutionPos * 0.5f;
	m_vDiff = m_vLook - vResolutionCenterPosition;
}

void CCamera::Render(HDC _hDC)
{
}

void CCamera::SetMainCamera()
{
	CCamera* pcam = MainCamera;
	if(nullptr != pcam)
		DeleteObject(pcam);
	CSceneManager::GetInstance()->GetCurScene()->SetMainCamera(this);
}