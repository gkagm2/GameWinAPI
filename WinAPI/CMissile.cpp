#include "stdafx.h"
#include "CMissile.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CScene.h"

#include "CCore.h" //Test

CMissile::CMissile(E_GroupType e_GroupType) :
	CObject(e_GroupType),
	m_fSpeed(100.0f),
	m_eMissileType(E_MissileType::NORMAL),
	m_fDirDegree(135),
	m_fLivingMaxTime(3.0f),
	m_fLivingTime(0.0f),
	m_pTargetObject(nullptr),
	m_testCoolTime(0.0f),
	m_testMaxCoolTime(0.1f),
	m_fPrevRadian(0.0f)
{
}

CMissile::~CMissile()
{
}

float CMissile::GetTargetAngle(const Vector3& _vPos, const Vector3& _vTargetPos)
{
	Vector3 vTargetPos = _vTargetPos;
	Vector3 vPosition = _vPos;
	Vector3 vDirVec = vTargetPos - vPosition;
	vDirVec.Normalized();
	vDirVec.y *= -1;
	float fTargetAngle = CMyMath::VectorToDegree(vDirVec);

	return fTargetAngle;
}

void CMissile::SetMissileType(E_MissileType _eMissileType)
{
	m_eMissileType = _eMissileType;
	switch (m_eMissileType) {
	case E_MissileType::GUIDED:
		m_fLivingMaxTime = 99.0f;
		break;
	}
}

void CMissile::SetTargetObject(CObject* _pTargetObject)
{
	m_pTargetObject = _pTargetObject;
}

void CMissile::Update()
{
	m_fLivingTime += DeltaTime;
	if (m_fLivingTime >= m_fLivingMaxTime) {
		DestroyObject(this);
		return;
	}

	switch (m_eMissileType) {
	case E_MissileType::NORMAL:
	{
		Vector3 vPosition = GetPosition();
		float fRadian = CMyMath::DegreeToRadian(m_fDirDegree);
		SetPosition(vPosition.x + cosf(fRadian) * m_fSpeed * DeltaTime, vPosition.y - sinf(fRadian) * m_fSpeed * DeltaTime);

	}
		break;
	case E_MissileType::GUIDED:
	{
		if (nullptr == m_pTargetObject)
			break;
		

		float fHeadAngle = CMyMath::RadianToDegree(m_fPrevRadian);
		float fTargetAngle = GetTargetAngle(GetPosition(), m_pTargetObject->GetPosition());

		HDC hdc = CCore::GetInstance()->GetDC();
		wchar_t b[255];
		swprintf(b, 255, L"fHeadAngle %f", fHeadAngle);
		TextOut(hdc, GetPosition().x, GetPosition().y + 20, b, wcslen(b));

		swprintf(b, 255, L"fTargetAngle %f", fTargetAngle);
		TextOut(hdc, GetPosition().x, GetPosition().y + 50, b, wcslen(b));


		float temp = fTargetAngle - fHeadAngle;
		if (fabsf(temp) > 180.f) {
			if (fTargetAngle <= fHeadAngle)
				fHeadAngle += DeltaTime * 90.0f;
			else
				fHeadAngle -= DeltaTime * 90.0f;
		}
		else {
			if (fTargetAngle >= fHeadAngle)
				fHeadAngle += DeltaTime * 90.0f;
			else
				fHeadAngle -= DeltaTime * 90.0f;
		}

		if (fHeadAngle >= 360.0f)
			fHeadAngle = 0.0f;
		else if (fHeadAngle <= -360.0f)
			fHeadAngle = 360.0f;
		
		m_fPrevRadian = CMyMath::DegreeToRadian(fHeadAngle);
		
		SetPosition(GetPosition().x + cosf(m_fPrevRadian) * m_fSpeed * DeltaTime,  GetPosition().y - sinf(m_fPrevRadian) * m_fSpeed * DeltaTime);
		/*wchar_t b[255];
				swprintf(b, 255, L"fAngle %f", fAngle);
				TextOut(hdc, GetPosition().x, GetPosition().y, b, wcslen(b));

				swprintf(b, 255, L"prevAngle %f", fPrevAngle);
				TextOut(hdc, GetPosition().x, GetPosition().y + 20, b, wcslen(b));

				swprintf(b, 255, L"gap %f", fAngle - fPrevAngle);
				TextOut(hdc, GetPosition().x, GetPosition().y + 40, b, wcslen(b));*/




	}

		break;
	case E_MissileType::UPGRADE:

		break;
	}
	// check all of monster object from 
}
