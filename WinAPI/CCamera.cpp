#include "stdafx.h"
#include "CCamera.h"
#include "CCore.h"
#include "CKeyManager.h"
#include "CTimeManager.h"

CCamera::CCamera() :
	m_vLook{0.f, 0.f, 0.f },
	m_pTargetObject(nullptr),
	m_fSpeed(200.0f),
	m_vDiff{0.f, 0.f, 0.f}
{
}

CCamera::~CCamera()
{
}

void CCamera::Update()
{
	Move();

	Vector3 resolutionPos = CCore::GetInstance()->GetResolution();
	Vector3 vResolutionCenterPosition = resolutionPos / 2.0f;
	m_vDiff = m_vLook - vResolutionCenterPosition;
}

void CCamera::Move()
{
	CKeyManager* keyManager = CKeyManager::GetInstance();
	if (keyManager->GetKeyState(E_Key::W) == E_KeyState::HOLD) {
		m_vLook.y -= m_fSpeed * DeltaTime;
	}
	if (keyManager->GetKeyState(E_Key::A) == E_KeyState::HOLD) {
		m_vLook.x -= m_fSpeed * DeltaTime;
	}
	if (keyManager->GetKeyState(E_Key::S) == E_KeyState::HOLD) {
		m_vLook.y += m_fSpeed * DeltaTime;
	}
	if (keyManager->GetKeyState(E_Key::D) == E_KeyState::HOLD) {
		m_vLook.x += m_fSpeed * DeltaTime;
	}
}