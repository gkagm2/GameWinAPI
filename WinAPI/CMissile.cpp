#include "stdafx.h"
#include "CMissile.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CScene.h"

CMissile::CMissile(E_GroupType e_GroupType) :
	CObject(e_GroupType),
	m_fSpeed(100.0f),
	m_eMissileType(E_MissileType::NORMAL),
	m_fDirDegree(135),
	m_fLivingMaxTime(3.0f),
	m_fLivingTime(0.0f)
{
}

CMissile::~CMissile()
{
}

void CMissile::Update()
{
	m_fLivingTime += DeltaTime;
	if (m_fLivingTime >= m_fLivingMaxTime) {
 		DestroyObject(this);
 		return;
	}
		
	Vector3 vPosition = GetPosition();
	float fRadian = CMyMath::DegreeToRadian(m_fDirDegree);
	SetPosition(vPosition.x + cosf(fRadian) * m_fSpeed * DeltaTime, vPosition.y - sinf(fRadian) * m_fSpeed * DeltaTime);
	// check all of monster object from 
}