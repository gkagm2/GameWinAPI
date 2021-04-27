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

CObject::CObject(E_GroupType _eGroupType = E_GroupType::DEFAULT) :
	m_vPosition{ 0, 0, 0 },
	m_vScale{ 100.0f, 100.0f, 100.0f },
	m_vRectPoint{},
	m_pTexture(nullptr),
	m_pCollider(nullptr),
	m_pAnimator(nullptr),
	m_pRigidbody(nullptr),
	m_eGroupType(_eGroupType),
	m_strName(STR_OBJECT_DEFAULT_NAME),
	m_bIsDead(false),
	m_bIsRender(true),
	m_bIsActive(true),
	m_fRotateDegree(0.f)
{
}
CObject::CObject(const CObject& _origin) :
	m_vPosition{ _origin.m_vPosition},
	m_vScale{ _origin.m_vScale },
	m_pTexture(_origin.m_pTexture),
	m_eGroupType(_origin.m_eGroupType),
	m_pCollider(nullptr),
	m_pAnimator(nullptr),
	m_pRigidbody(nullptr),
	m_strName(_origin.m_strName + L"_copy"),
	m_bIsDead(_origin.m_bIsDead),
	m_bIsRender(_origin.m_bIsRender),
	m_bIsActive(_origin.m_bIsActive),
	m_fRotateDegree(0.f)
{
	for (int i = 0; i < 3; ++i)
		m_vRectPoint[i] = _origin.m_vRectPoint[i];

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

void CObject::RenderRotatedTex(HDC _hDC, const Vector3& _vRenderPos, HDC _TextureDC, int xSrc, int ySrc, int width, int height)
{
	POINT rPNT[3];
	// 애니메이션 텍스쳐일경우.
	Vector3 v4;
	if (GetAnimator())
		v4 = { (float)GetAnimator()->GetAnimTexWidth(), (float)GetAnimator()->GetAnimTexHeight(), 0.f };
	else
		v4 = { (float)GetTexture()->GetWidth() * 0.5f, (float)GetTexture()->GetHeight() * 0.5f, 0.f };
	
	v4.Normalized();
	v4 = Rotate(v4, GetRotateDegree());
	v4.x = v4.x * GetScale().x;
	v4.y = v4.y * GetScale().y;
	v4.z = v4.z * GetScale().z;

	rPNT[0].x = (int)(GetRectPoint(0).x);
	rPNT[0].y = (int)(GetRectPoint(0).y);
	rPNT[1].x = (int)(GetRectPoint(1).x);
	rPNT[1].y = (int)(GetRectPoint(1).y);
	rPNT[2].x = (int)(GetRectPoint(2).x);
	rPNT[2].y = (int)(GetRectPoint(2).y);

	POINT rPNT4; // 네번째 꼭지점의 위치값
	rPNT4.x = (int)(v4.x);
	rPNT4.y = (int)(v4.y);

	int minX = rPNT[0].x;
	int minY = rPNT[0].y;
	int maxX = rPNT[0].x;
	int maxY = rPNT[0].y;
	for (int i = 1; i < 3; ++i) {
		minX = min(rPNT[i].x, minX);
		minY = min(rPNT[i].y, minY);
		maxX = max(rPNT[i].x, maxX);
		maxY = max(rPNT[i].y, maxY);
	}
	minX = min(rPNT4.x, minX);
	maxX = max(rPNT4.x, maxX);
	minY = min(rPNT4.y, minY);
	maxY = max(rPNT4.y, maxY);

	CTexture* pNewTexture = new CTexture();
	pNewTexture->Create(maxX - minX, maxY - minY);
	Vector2 vCenter{ maxX + minX, maxY + minY };

	HBITMAP bitmap{};
	for (int i = 0; i < 3; ++i) {
		rPNT[i].x += maxX;
		rPNT[i].y += maxY;
	}
	// 회전
	PlgBlt(pNewTexture->GetDC(), rPNT, _TextureDC, xSrc, ySrc, width, height, bitmap, 0, 0);
	// 투명
	TransparentBlt(_hDC, _vRenderPos.x + minX, _vRenderPos.y + minY, maxX - minX, maxY - minY, pNewTexture->GetDC(), 0, 0, maxX - minX, maxY - minY, EXCEPTION_COLOR_RGB_BLACK);

	if (pNewTexture)
		delete pNewTexture;
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
	Vector3 dir;
	dir = m_vRectPoint[_idx];
	dir.Normalized();

	dir = Rotate(dir, m_fRotateDegree);

	dir.x *= GetScale().x;
	dir.y *= GetScale().y;
	dir.z *= GetScale().z;
	return dir;
}

void CObject::RotateRP(float _fDegree)
{
	m_fRotateDegree += _fDegree;
}

void CObject::RotateRP(const Vector3& _vTargetDir, const Vector3& _vHeadDir, float fDegree)
{
	Vector3 vCross = CMyMath::GetCross(_vTargetDir, _vHeadDir);
	float fTargetDirDegree = atan2f(_vTargetDir.y, _vTargetDir.x) * CMyMath::Rad2Deg(); // 타겟 방향으로 회전 한 각 구하기
	float fHeadDirDegree = atan2f(_vHeadDir.y, _vHeadDir.x) * CMyMath::Rad2Deg();
	if (fTargetDirDegree == fHeadDirDegree)
		return;
	if (vCross.z < 0)
		RotateRP(fDegree);
	else if (vCross.z > 0)
		RotateRP(-fDegree);
}

void CObject::LookAt(Vector3 _vTargetPos, float _fRotateSpeed)
{
	Vector3 vObjPos = GetPosition();
	Vector3 vLookAtDir = { _vTargetPos.x - vObjPos.x, _vTargetPos.y - vObjPos.y };
	vLookAtDir.Normalized();

	Vector3 vHeadDir = GetUpVector();

	float angle = atan2f(vLookAtDir.y, vLookAtDir.x) * CMyMath::Rad2Deg(); // 타겟 방향으로 회전 한 각 구하기

	SetRotateDegree(angle - 90);

	/*Vector3 vCross = CMyMath::GetCross(vLookAtDir, vHeadDir);
	if (vCross.z < 0)
		RotateRP(_fRotateSpeed * DeltaTime);
	else if (vCross.z > 0)
		RotateRP(-_fRotateSpeed * DeltaTime);*/
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

Vector3 CObject::GetUpVector()
{
	Vector3 vUpVec = { 0, -1, 0 };
	vUpVec = Rotate(vUpVec, m_fRotateDegree);
	return vUpVec;
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

void CObject::Save(FILE* _pFile)
{
	fwrite(&m_vPosition, sizeof(Vector3), 1, _pFile);
	fwrite(&m_vScale, sizeof(Vector3), 1, _pFile);
	for (int i = 0; i < 3; ++i)
		fwrite(&m_vRectPoint[i], sizeof(Vector3), 1, _pFile);

	SaveWString(m_strName, _pFile);
	fwrite(&m_bIsDead, sizeof(bool), 1, _pFile);
	fwrite(&m_bIsRender, sizeof(bool), 1, _pFile);
	fwrite(&m_bIsActive, sizeof(bool), 1, _pFile);
	fwrite(&m_fRotateDegree, sizeof(float), 1, _pFile);
}

void CObject::Load(FILE* _pFile)
{
	fread(&m_vPosition, sizeof(Vector3), 1, _pFile);
	fread(&m_vScale, sizeof(Vector3), 1, _pFile);
	for (int i = 0; i < 3; ++i)
		fread(&m_vRectPoint[i], sizeof(Vector3), 1, _pFile);

	LoadWString(m_strName, _pFile);
	fread(&m_bIsDead, sizeof(bool), 1, _pFile);
	fread(&m_bIsRender, sizeof(bool), 1, _pFile);
	fread(&m_bIsActive, sizeof(bool), 1, _pFile);
	fread(&m_fRotateDegree, sizeof(float), 1, _pFile);
}