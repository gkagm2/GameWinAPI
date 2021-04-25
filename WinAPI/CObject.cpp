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
	m_bIsActive(true),
	m_fRotateAngle(0.f)
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
	m_bIsActive(_origin.m_bIsActive),
	m_fRotateAngle(0.f)
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

			const Vector3& vScale = GetScale();
			// 예외처리할 색상 RGB값을 처리하기 위해 BitBlt대신 TransparentBlt을 이용 (library 필요)
			TransparentBlt(
				_hDC,
				(int)(vRenderPosition.x - vScale.x * 0.5f), (int)(vRenderPosition.y - vScale.y * 0.5f),
				(int)vScale.x, (int)vScale.y,
				m_pTexture->GetDC(),
				0, 0,
				m_pTexture->GetWidth(), m_pTexture->GetHeight(),
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
		if(GetCollider()->IsRender())
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

void CObject::RotateRP(Vector3 _vDir)
{
#pragma region direction Rendering (Test)
	/*Vector3 rendPos = MainCamera->GetRenderPosition(GetCharacter()->GetPosition());
	HDC hdc = CCore::GetInstance()->GetDC();
	MoveToEx(hdc, rendPos.x, rendPos.y, nullptr);
	LineTo(hdc, rendPos.x + vHeadDir.x * 70.f, rendPos.y + vHeadDir.y * 70.f);
	MoveToEx(hdc, rendPos.x, rendPos.y, nullptr);
	LineTo(hdc, rendPos.x + vNextPathDir.x * 100.f, rendPos.y + vNextPathDir.y * 100.f);*/
#pragma endregion
	Vector3 vHeadDir = GetUpVector();
	_vDir.y *= -1.f;
	vHeadDir.y *= -1.f;

	float vDot = CMyMath::GetDot(_vDir, vHeadDir);
	if (isnan(vDot))
		return;
	Vector3 vCross = CMyMath::GetCross(_vDir, vHeadDir);
	if (vCross.z < 0) {
		if (vCross.z > -0.1f)
			RotateRP(100 * DeltaTime);
		else
			RotateRP(350 * DeltaTime);
	}
	else if (vCross.z > 0) {
		if (vCross.z < 0.1f)
			RotateRP(-100 * DeltaTime);
		else
			RotateRP(-350 * DeltaTime);
	}
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

void CObject::SetActive(bool _bIsActive)
{
	m_bIsActive = _bIsActive;
	if (GetCollider())
		GetCollider()->SetActive(_bIsActive);
	if (GetRigidbody())
		GetRigidbody()->SetActive(_bIsActive);
}

void CObject::LookAt(Vector3 _vTargetPos, float _fRotateSpeed)
{
	Vector3 vObjPos = GetPosition();
	Vector3 vLookAtDir = { _vTargetPos.x - vObjPos.x, _vTargetPos.y - vObjPos.y };
	vLookAtDir.Normalized();

	Vector3 vHeadDir = GetUpVector();
	//vLookAtDir.y *= -1.f;
	//vHeadDir.y *= -1.f;

	float vDot = CMyMath::GetDot(vLookAtDir, vHeadDir);
	if (isnan(vDot))
		assert(nullptr && L"float value is nan");
	Vector3 vCross = CMyMath::GetCross(vLookAtDir, vHeadDir);
	if (vCross.z < 0)
		RotateRP(_fRotateSpeed);
	else if (vCross.z > 0)
		RotateRP(-_fRotateSpeed);
}

void CObject::Save(FILE* _pFile)
{
	fwrite(&m_vPosition, sizeof(Vector3), 1, _pFile);
	fwrite(&m_vScale, sizeof(Vector3), 1, _pFile);
	for (int i = 0; i < 3; ++i)
		fwrite(&m_vRectPoint[i], sizeof(Vector3), 1, _pFile);
	for (int i = 0; i < 3; ++i)
		fwrite(&m_vRPDir[i], sizeof(Vector3), 1, _pFile);

	SaveWString(m_strName, _pFile);
	fwrite(&m_bIsDead, sizeof(bool), 1, _pFile);
	fwrite(&m_bIsRender, sizeof(bool), 1, _pFile);
	fwrite(&m_bIsActive, sizeof(bool), 1, _pFile);
	fwrite(&m_fRotateAngle, sizeof(float), 1, _pFile);
}

void CObject::Load(FILE* _pFile)
{
	fread(&m_vPosition, sizeof(Vector3), 1, _pFile);
	fread(&m_vScale, sizeof(Vector3), 1, _pFile);
	for (int i = 0; i < 3; ++i)
		fread(&m_vRectPoint[i], sizeof(Vector3), 1, _pFile);
	for (int i = 0; i < 3; ++i)
		fread(&m_vRPDir[i], sizeof(Vector3), 1, _pFile);

	LoadWString(m_strName, _pFile);
	fread(&m_bIsDead, sizeof(bool), 1, _pFile);
	fread(&m_bIsRender, sizeof(bool), 1, _pFile);
	fread(&m_bIsActive, sizeof(bool), 1, _pFile);
	fread(&m_fRotateAngle, sizeof(float), 1, _pFile);
}