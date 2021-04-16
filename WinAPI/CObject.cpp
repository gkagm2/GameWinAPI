#include "stdafx.h"
#include "CObject.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CCollider.h"
#include "CSceneManager.h"
#include "CCollisionManager.h"
#include "CScene.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CCamera.h"
#include "CCameraManager.h"
#include "CSceneManager.h"
#include "CTimeManager.h"

CObject::CObject(E_GroupType e_GroupType = E_GroupType::DEFAULT) :
	m_vPosition{ 0, 0, 0 },
	m_vScale{ 100.0f, 100.0f, 100.0f },
	m_vRectPoint{},
	m_vRPDir{},
	m_vUpVec{ 0.f, -1.f , 0.f },
	m_pTexture(nullptr),
	m_pCollider(nullptr),
	m_pAnimator(nullptr),
	m_pRigidbody(nullptr),
	m_eGroupType(e_GroupType),
	m_strName(STR_OBJECT_DEFAULT_NAME),
	m_bIsDead(false),
	m_bIsRender(true),
	m_bIsActive(true)
{
}

void CObject::Save()
{
}

void CObject::Load()
{
}

CObject::CObject(const CObject& _origin) :
	m_vPosition{ _origin.m_vPosition},
	m_vScale{ _origin.m_vScale },
	m_vUpVec{ _origin.m_vUpVec },
	m_pTexture(_origin.m_pTexture),
	m_eGroupType(_origin.m_eGroupType),
	m_pCollider(nullptr),
	m_pAnimator(nullptr),
	m_pRigidbody(nullptr),
	m_strName(_origin.m_strName + L"_copy"),
	m_bIsDead(_origin.m_bIsDead),
	m_bIsRender(_origin.m_bIsRender),
	m_bIsActive(_origin.m_bIsActive)
{
	for (int i = 0; i < 3; ++i) {
		m_vRectPoint[i] = _origin.m_vRectPoint[i];
		m_vRPDir[i] = _origin.m_vRPDir[i];
	}

	if (_origin.m_pCollider) {
		m_pCollider = _origin.m_pCollider->Clone();
		m_pCollider->m_pOwnerObject = this;
	}
	if (_origin.m_pAnimator) {
		m_pAnimator = _origin.m_pAnimator->Clone();
		m_pAnimator->m_pOwner = this;
	}

	if (_origin.m_pRigidbody) {
		m_pRigidbody = _origin.m_pRigidbody->Clone();
		m_pRigidbody->m_pOwnerObject = this;
	}
}

CObject::~CObject()
{
   	if (nullptr != m_pCollider)
		delete m_pCollider;
	if (nullptr != m_pAnimator)
		delete m_pAnimator;
	if (nullptr != m_pRigidbody)
		delete m_pRigidbody;
}

void CObject::Init()
{
	InitRectPoint();
}

void CObject::PrevUpdate()
{
	if (nullptr != m_pRigidbody)
		m_pRigidbody->PrevUpdate();
}

void CObject::LateUpdate()
{
	if (nullptr != m_pAnimator)
		m_pAnimator->LateUpdate();
	if (nullptr != m_pCollider)
		m_pCollider->LateUpdate();
	if (nullptr != m_pRigidbody)
		m_pRigidbody->LateUpdate();
}

void CObject::Render(HDC _hDC)
{
	Vector3 vRenderPosition = MainCamera->GetRenderPosition(m_vPosition);

	if (nullptr == m_pTexture) {
		Rectangle(_hDC,
			(int)(vRenderPosition.x - ScaleX() * 0.5f),
			(int)(vRenderPosition.y - ScaleY() * 0.5f),
			(int)(vRenderPosition.x + ScaleX() * 0.5f),
			(int)(vRenderPosition.y + ScaleY() * 0.5f));
	}
	else {

		if (nullptr != m_pAnimator)
			m_pAnimator->Render(_hDC);
		else {
			UINT iWidth = (UINT)ScaleX();
			UINT iHeight = (UINT)ScaleY();
			UINT iWidth1 = m_pTexture->GetWidth();
			UINT iHeight1 = m_pTexture->GetHeight();
			HDC hTextureDC = m_pTexture->GetDC();

			// 예외처리할 색상 RGB값을 처리하기 위해 BitBlt대신 TransparentBlt을 이용 (library 필요)
			TransparentBlt(
				_hDC,
				(int)(vRenderPosition.x - iWidth * 0.5f), (int)(vRenderPosition.y - iHeight * 0.5f),
				iWidth, iHeight,
				hTextureDC,
				0, 0,
				iWidth1, iHeight1,
				(COLORREF)EXCEPTION_COLOR_RGB_BLACK); // 제거 할 색상

			/*BitBlt(
				_hDC,
				(int)(m_vPos.x - iWidth * 0.5f),
				(int)(m_vPos.y - iHeight * 0.5f),
				iWidth, iHeight,
				hTextureDC,
				0, 0,
				SRCCOPY);*/
			// Print Position
		}
	}

	// ColliderRendering
	if (nullptr != m_pCollider) {
		m_pCollider->Render(_hDC);
	}
}

void CObject::InitRectPoint()
{
	float fWidth = 50.f;
	float fHeight = 50.f;
	if (nullptr != m_pAnimator) {
		fWidth = m_pAnimator->GetAnimTexWidth();
		fHeight = m_pAnimator->GetAnimTexHeight();
	}
	else {
		if (nullptr != m_pTexture) {
			fWidth = (float)m_pTexture->GetWidth();
			fHeight = (float)m_pTexture->GetHeight();
		}
	}

	Vector3 pos = GetPosition();
	Vector3 vLeftTop = { 0.f - fWidth * 0.5f, vLeftTop.y = 0.f - fHeight * 0.5f };
	Vector3 vRightTop = { 0.f + fWidth * 0.5f, 0.f - fHeight * 0.5f };
	Vector3 vLeftBottom = { 0.f - fWidth * 0.5f, 0.f + fHeight * 0.5f };

	SetRectPoint(0, vLeftTop);
	SetRectPoint(1, vRightTop);
	SetRectPoint(2, vLeftBottom);
}

const Vector3& CObject::GetRectPoint(int _idx)
{
	m_vRectPoint[_idx].Normalized();
	m_vRectPoint[_idx].x *= GetScale().x;
	m_vRectPoint[_idx].y *= GetScale().y;
	m_vRectPoint[_idx].z *= GetScale().z;
	return m_vRectPoint[_idx];
}

void CObject::RotateRP(float _fDegree)
{
	m_vUpVec = Rotate(m_vUpVec, _fDegree);

	Vector3 dir[3];
	dir[0] = m_vRectPoint[0];
	dir[1] = m_vRectPoint[1];
	dir[2] = m_vRectPoint[2];
	dir[0].Normalized();
	dir[1].Normalized();
	dir[2].Normalized();

	dir[0] = Rotate(dir[0], _fDegree);
	dir[1] = Rotate(dir[1], _fDegree);
	dir[2] = Rotate(dir[2], _fDegree);

	Vector3 rp0 = GetRectPoint(0);
	Vector3 rp1 = GetRectPoint(1);
	Vector3 rp2 = GetRectPoint(2);
	SetRectPoint(0, dir[0] * rp0.GetDistance());
	SetRectPoint(1, dir[1] * rp1.GetDistance());
	SetRectPoint(2, dir[2] * rp2.GetDistance());
}

Vector3 CObject::GetMax()
{
	Vector3 maxVec = m_vPosition;
	if (nullptr != m_pTexture) {
		maxVec.x += m_pTexture->GetWidth() * 0.5f;
		maxVec.y += m_pTexture->GetHeight() * 0.5f;
	}
	else
		maxVec += m_vScale * 0.5f;
	return maxVec;
}

void CObject::SetUpVector(const Vector3& _upVector, const Vector3 _vRPDir[3], const Vector3 _vRectPoint[3])
{
	m_vUpVec = _upVector;
	for (int i = 0; i < 3; ++i) {
		m_vRPDir[i] = _vRPDir[i];
		m_vRectPoint[i] = _vRectPoint[i];
	}
}

void CObject::SetTexture(CTexture* _pTexture)
{
	m_pTexture = _pTexture;
	//m_vScale.x = (float)m_pTexture->GetWidth();
	//m_vScale.y = (float)m_pTexture->GetHeight();
}

float CObject::GetTextureWidth()
{
	return nullptr != m_pTexture ? (float)m_pTexture->GetWidth() : 0.0f;
}

float CObject::GetTextureHeight()
{
	return nullptr != m_pTexture ? (float)m_pTexture->GetHeight() : 0.0f;
}

Vector3 CObject::GetMin()
{
	Vector3 minVec = m_vPosition;
	if (nullptr != m_pTexture) {
		minVec.x -= m_pTexture->GetWidth() * 0.5f;
		minVec.y -= m_pTexture->GetHeight() * 0.5f;
	}
	else
		minVec -= m_vScale * 0.5f;
	return minVec;
}