#include "stdafx.h"
#include "CMonster.h"
#include "CTimeManager.h"
#include "CCore.h"
#include "CMissile.h"
#include "CColliderCircle.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CPlayer.h"
#include "CCamera.h"
#include "CAnimator.h"
#include "CTexture.h"
#include "CAI.h"

enum class E_MissileType;

CMonster::CMonster(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_fSpeed(1.0f),
	m_fDirection(1.0f),
	m_fRange(800.0f),
	m_vStartPosition(0, 0, 0),
	m_fRatioX(0),
	m_fRatioY(0),
	m_eUpgradeLevel(E_UpgradeLevelType::LEVEL1),
	m_fHitEffectMaxCoolTime(0.4f),
	m_fHitEffectCoolTime(0.0f),
	m_bIsHit(false),
	m_vOriginalScale(GetScale()),
	m_fMissileFireCoolTime(0.0f),
	m_fMissileMaxFireCoolTime(3.0f),
	m_fMissileSpeed(400.0f),
	m_pTargetObj(nullptr),
	m_pAI(nullptr)
{
	m_vOriginalScale = GetScale();

	// AI
}

CMonster::CMonster(const CMonster& _other) :
	CObject(_other.m_eGroupType),
	m_fSpeed(_other.m_fSpeed),
	m_fDirection(_other.m_fDirection),
	m_fRange(_other.m_fRange),
	m_vStartPosition(_other.m_vStartPosition),
	m_fRatioX(_other.m_fRatioX),
	m_fRatioY(_other.m_fRatioY),
	m_eUpgradeLevel(_other.m_eUpgradeLevel),
	m_fHitEffectMaxCoolTime(_other.m_fHitEffectCoolTime),
	m_fHitEffectCoolTime(_other.m_fHitEffectCoolTime),
	m_bIsHit(_other.m_bIsHit),
	m_vOriginalScale(_other.m_vOriginalScale),
	m_fMissileFireCoolTime(_other.m_fMissileFireCoolTime),
	m_fMissileMaxFireCoolTime(_other.m_fMissileMaxFireCoolTime),
	m_fMissileSpeed(_other.m_fMissileSpeed),
	m_pTargetObj(_other.m_pTargetObj),
	m_pAI(nullptr)
{
	if (nullptr != _other.m_pAI)
		m_pAI = _other.m_pAI->Clone();
}

CMonster::~CMonster()
{
}

void CMonster::Update()
{
	// Shoot Missile 
	m_fMissileFireCoolTime += DeltaTime;
	if (m_fMissileFireCoolTime >= m_fMissileMaxFireCoolTime) {
		float fRandomMaxCoolTime = (float)((rand() % 2) + 0.2f);
		int eRandomFireLevel = rand() % (int)E_UpgradeLevelType::END;
		m_eUpgradeLevel = (E_UpgradeLevelType)eRandomFireLevel;
		m_fMissileMaxFireCoolTime = fRandomMaxCoolTime;
		FireMissile();

		m_fMissileFireCoolTime = 0.0f;
	}
	
	// Hit Effect
	if (m_bIsHit) {
 		Vector3 vScale = GetScale();
		vScale = vScale - 80.0f * DeltaTime;
		if (vScale.x <= m_vOriginalScale.x) {
			vScale = m_vOriginalScale;
			m_bIsHit = false;
		}
		SetScale(vScale);
	}

	// Move
	Move(2);
}

void CMonster::Render(HDC _hDC)
{
	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	if (nullptr == GetTexture()) {
		Rectangle(_hDC,
			(int)(vRenderPosition.x - ScaleX() * 0.5f),
			(int)(vRenderPosition.y - ScaleY() * 0.5f),
			(int)(vRenderPosition.x + ScaleX() * 0.5f),
			(int)(vRenderPosition.y + ScaleY() * 0.5f));


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

		if (nullptr != GetAnimator())
			GetAnimator()->Render(_hDC);
		else {
			UINT iWidth = (UINT)ScaleX();
			UINT iHeight = (UINT)ScaleY();
			UINT iWidth1 = GetTexture()->GetWidth();
			UINT iHeight1 = GetTexture()->GetHeight();
			HDC hTextureDC = GetTexture()->GetDC();

			//TransparentBlt(
			//	_hDC,
			//	(int)(vRenderPosition.x - iWidth * 0.5f), (int)(vRenderPosition.y - iHeight * 0.5f),
			//	iWidth, iHeight,
			//	hTextureDC,
			//	0, 0,
			//	iWidth1, iHeight1,
			//	(COLORREF)EXCEPTION_COLOR_RGB); // 제거 할 색상

			// 예외처리할 색상 RGB값을 처리하기 위해 BitBlt대신 TransparentBlt을 이용 (library 필요)


			BLENDFUNCTION tBlendFunc;
			tBlendFunc.BlendOp = AC_SRC_OVER;
			tBlendFunc.BlendFlags = 0;					// 반드시 0
			tBlendFunc.SourceConstantAlpha = 120;	// 0 : 투명, 255 불투명
			tBlendFunc.AlphaFormat = AC_SRC_ALPHA;					// AC_SRC_ALPHA
			
			AlphaBlend(_hDC,
				(int)(vRenderPosition.x - iWidth * 0.5f), (int)(vRenderPosition.y - iHeight * 0.5f), (int)iWidth, (int)iHeight,
				GetTexture()->GetDC(),
				0, 0, (int)iWidth1, (int)iHeight1,
				tBlendFunc);
		}
	}

	// ColliderRendering
	if (nullptr != GetCollider()) {
		if (GetCollider()->IsRender())
			GetCollider()->Render(_hDC);
	}
}

void CMonster::OnCollisionEnter(CObject* _pOther)
{
  	CMissile* pMissile = dynamic_cast<CMissile*>(_pOther);
	if (pMissile) {
		E_GroupType groupType = pMissile->GetGroupType();

		m_bIsHit = true;
		SetScale(GetScale() + 30);
		if (GetScale().x > m_vOriginalScale.x * 4.0f)
			DestroyObject(this);
	}
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther);
	if (pPlayer) {
		E_GroupType  groupType = pPlayer->GetGroupType();
		DestroyObject(this);
	}
}

void CMonster::CreateMissile(float _fSpeed, Vector3 _vNozzlePosition, Vector3 _vDirVec, E_MissileType _eMissileType = E_MissileType::NORMAL)
{
	CMissile* pMissile = new CMissile(E_GroupType::MONSTER_PROJECTILE);
	pMissile->SetMissileType(_eMissileType);
	pMissile->SetSpeed(_fSpeed);
	pMissile->SetPosition(_vNozzlePosition);
	pMissile->SetScale(Vector3(10.0f, 10.0f));
	pMissile->SetDirectionVector(_vDirVec);

	switch (_eMissileType) {
	case E_MissileType::NORMAL:
		pMissile->SetObjectName(L"Monster Misisle");
		break;
	case E_MissileType::GUIDED:
	{
		pMissile->SetObjectName(L"Monster Guided Missile");
		if (nullptr != m_pTargetObj)
			pMissile->SetTargetObject(m_pTargetObj);
	}
		break;
	}

	// 미사일 충돌 컴포넌트 생성 및 추가
	CColliderCircle* pMissileCircleCollider = new CColliderCircle(pMissile);
	CreateObject(pMissile);
}

void CMonster::FireMissile()
{
	Vector3 vPosition = GetPosition();
	Vector3 vNozzlePosition(vPosition.x, vPosition.y + ScaleY() * 0.5f);

	assert(nullptr != m_pTargetObj);
	Vector3 vMonsterPos = GetPosition();
	Vector3 vTargetPos = m_pTargetObj->GetPosition();
	Vector3 vDir = vTargetPos - vMonsterPos;
	vDir.Normalized();
	vDir.y *= -1;

	switch (m_eUpgradeLevel) {
	case E_UpgradeLevelType::LEVEL1:
	{
		m_fMissileSpeed = 200.0f;
		CreateMissile(m_fMissileSpeed, vNozzlePosition, vDir, E_MissileType::GUIDED);
	}
	break;
	case E_UpgradeLevelType::LEVEL2:
	{
		Vector3 vLeftNozzlePosition(vNozzlePosition.x - ScaleX() * 0.5f, vNozzlePosition.y);
		Vector3 vRightNozzlePosition(vNozzlePosition.x + ScaleX() * 0.5f, vNozzlePosition.y);
		CreateMissile(m_fMissileSpeed, vLeftNozzlePosition, vDir);
		CreateMissile(m_fMissileSpeed, vRightNozzlePosition, vDir);
	}
	break;
	case E_UpgradeLevelType::LEVEL3:
	{
		Vector3 vLeftNozzlePosition(vNozzlePosition.x - ScaleX() * 0.5f, vNozzlePosition.y);
		Vector3 vRightNozzlePosition(vNozzlePosition.x + ScaleX() * 0.5f, vNozzlePosition.y);
		CreateMissile(m_fMissileSpeed, vRightNozzlePosition, Rotate(vDir, 15.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, vDir);
		CreateMissile(m_fMissileSpeed, vLeftNozzlePosition, Rotate(vDir, -15.0f));
	}
	break;
	case E_UpgradeLevelType::LEVEL4:
	{
		CreateMissile(m_fMissileSpeed, vNozzlePosition, Rotate(vDir, -15.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, Rotate(vDir, +15.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, Rotate(vDir, -30.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, Rotate(vDir, +30.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, Rotate(vDir, -45.0f));
		CreateMissile(m_fMissileSpeed, vNozzlePosition, Rotate(vDir, +45.0f) );
		CreateMissile(m_fMissileSpeed, vNozzlePosition, vDir);
	}
	break;
	default:
		break;
	}
}

void CMonster::Move(int _iType = 0)
{
	_iType = _iType < 0 ? max(_iType, 0) : min(_iType, 3);

	switch (_iType) {
	case 0:
		// just static
		break;
	case 1:
		// move1
	{
		Vector3 startPosition = m_vStartPosition;
		m_fRatioX += DeltaTime * m_fSpeed;
		float offsetX = cosf(m_fRatioX) * m_fRange * 0.5f;
		SetPosition(m_vStartPosition.x + offsetX, m_vStartPosition.y);
	}
		break;
	case 2:
		// move2
	{
		Vector3 targetPosition = m_pTargetObj->GetPosition();
		Vector3 vMovedPosition = targetPosition;
		m_fRatioX += DeltaTime * m_fSpeed;
		m_fRatioY += DeltaTime * 3.0f;
		float offsetX = cosf(m_fRatioX) * m_fRange * 0.5f;
		float offsetY = sinf(m_fRatioY) * 150.0f * 0.5f;
		SetPosition(m_vStartPosition.x + offsetX, m_vStartPosition.y + offsetY);
	}
		break;
	case 3:
		break;
	}
}

void CMonster::CreateAI()
{
	m_pAI = new CAI;
	m_pAI->m_pMonster = this;
}
