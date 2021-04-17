#include "stdafx.h"
#include "CResourceManager.h"
#include "CGTA_Player.h"
#include "CColliderRect.h"
#include "CTexture.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CKeyManager.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CCamera.h"
#include "CGTA_Character.h"
#include "CGTA_Bullet.h"
#include "CRigidbody2D.h"
#include "CGTA_Item.h"

CGTA_Player::CGTA_Player(E_GroupType _eGroupType) :
	CGTA_Character(_eGroupType),
	m_bIsDrive(false),
	m_fAttackCoolTime(0.f),
	m_fAttackMaxCoolTime(0.f)
{
}

CGTA_Player::~CGTA_Player()
{
}

void CGTA_Player::Init()
{
	// Texture  set
	CTexture* pTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_gta_player);
	if (nullptr == pTexture)
		pTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_gta_player, STR_FILE_PATH_gta_player);
	SetTexture(pTexture);

	// Animator set
	SetAnimator(new CAnimator(this));
	GetAnimator()->CreateAnimation(L"idle", pTexture, Vector2(0, 40 * 0), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"idle_gun", pTexture, Vector2(0,40 * 1), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"stun", pTexture, Vector2(0,40 * 2), Vector2(40, 40), 1, 0.3f);

	GetAnimator()->CreateAnimation(L"dead1", pTexture, Vector2(0, 40 * 3), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"dead2", pTexture, Vector2(0, 40 * 4), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"dead3", pTexture, Vector2(0, 40 * 5), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"dead4", pTexture, Vector2(0, 40 * 6), Vector2(40, 40), 1, 0.3f);
	GetAnimator()->CreateAnimation(L"dead5", pTexture, Vector2(0, 40 * 7), Vector2(40, 40), 1, 0.3f);

	GetAnimator()->CreateAnimation(L"creash", pTexture, Vector2(0,40 * 8), Vector2(40, 40), 2, 0.07f);
	GetAnimator()->CreateAnimation(L"run_gun", pTexture, Vector2(0,40 * 9), Vector2(40, 40), 8, 0.07f);
	GetAnimator()->CreateAnimation(L"walk_gun", pTexture, Vector2(0,40 * 10), Vector2(40, 40), 3, 0.07f);
	GetAnimator()->CreateAnimation(L"punch", pTexture, Vector2(0,40 * 11), Vector2(40, 40), 6, 0.07f);
	GetAnimator()->CreateAnimation(L"run", pTexture, Vector2(0,40 * 12), Vector2(40, 40), 8, 0.07f);

	GetAnimator()->PlayAnimation(L"idle", E_AnimationPlayType::ONCE);

	float fAnimTextureWidth = GetAnimator()->GetAnimTexWidth();
	float fAnimTextureHeight = GetAnimator()->GetAnimTexHeight();
	Vector2 vScale{ fAnimTextureWidth, fAnimTextureHeight };
	SetScale(Vector3(vScale.x, vScale.y, 0.0f));

	// Collider set
	CColliderRect* pCollider = new CColliderRect(this);
	SetCollider(pCollider);
	pCollider->SetScale(Vector3(15.f, 15.f, 15.f));

	CRigidbody2D* pRigidbody = new CRigidbody2D(this);

	SetPosition(0, 0, 0);
	
	CGTA_Character::Init();
}

void CGTA_Player::PrevUpdate()
{
	__super::PrevUpdate();
}

void CGTA_Player::Update()
{

	if (m_bIsDrive)
		Drive();
	else
		Move();

	if (InputKeyPress(E_Key::Ctrl)) {
		m_fAttackCoolTime += DeltaTime;
		if (m_fAttackCoolTime >= m_fAttackMaxCoolTime) {
			Attack();
			m_fAttackCoolTime = 0.f;
		}
	}

	if (InputKeyPress(E_Key::Z)) {
		ChangePrevWeapon();
	}
	if (InputKeyPress(E_Key::X)) {
		ChangeNextWeapon();
	}
}

void CGTA_Player::LateUpdate()
{
	__super::LateUpdate();
}

void CGTA_Player::Render(HDC _hDC)
{
	CGTA_Character::Render(_hDC);
}

void CGTA_Player::OnCollisionEnter(CObject* _pOther)
{
}

void CGTA_Player::OnCollisionStay(CObject* _pOther)
{
}

void CGTA_Player::OnCollisionExit(CObject* _pOther)
{
}

void CGTA_Player::Move()
{
	Vector3 vHeadDir = GetUpVector();
	//RotateInfo().Update();
	if (InputKeyHold(E_Key::LEFT)) {
		RotateRP(-220 * DeltaTime);
	}
	if (InputKeyHold(E_Key::RIGHT)) {
		RotateRP(220 * DeltaTime);
	}

	if (InputKeyPress(E_Key::UP)) {
		GetAnimator()->PlayAnimation(L"run", E_AnimationPlayType::LOOP);
	}
	if (InputKeyPress(E_Key::DOWN)) {
		GetAnimator()->PlayAnimation(L"run", E_AnimationPlayType::LOOP);
	}
	if (InputKeyRelease(E_Key::UP)) {
		GetAnimator()->PlayAnimation(L"idle", E_AnimationPlayType::ONCE);
	}
	if (InputKeyRelease(E_Key::DOWN)) {
		GetAnimator()->PlayAnimation(L"idle", E_AnimationPlayType::ONCE);
	}

	if (InputKeyHold(E_Key::UP)) {
		float x = GetPosition().x - GetUpVector().x * 300 * DeltaTime;
		float y = GetPosition().y - GetUpVector().y * 300 * DeltaTime;
		SetPosition(GetPosition().x - GetUpVector().x * 300 * DeltaTime, GetPosition().y - GetUpVector().y * 300 * DeltaTime);
	}
	if (InputKeyHold(E_Key::DOWN)) {
		SetPosition(GetPosition().x + GetUpVector().x * 300 * DeltaTime, GetPosition().y + GetUpVector().y * 300 * DeltaTime);
	}
}

void CGTA_Player::Attack()
{
	TWeaponInfo& tWeaponInfo = m_vecWeapon[(UINT)m_eCurWeaponType].second;

	E_WeaponType eWeaponType = GetCurWeaponType();

	// �ָ��̸�
	if (E_WeaponType::FIST == eWeaponType) {
		GetAnimator()->PlayAnimation(L"punch", E_AnimationPlayType::ONCE);
		// Punch �ö��̴��� �ϳ� �����Ǿ��ٰ� �������.
	
	} 
	else {	
		// �Ѿ� ������Ʈ ����.
		CGTA_Bullet* pBullet = new CGTA_Bullet(E_GroupType::PROJECTILE);
		pBullet->Init();
		pBullet->SetPosition(GetPosition() + GetNozzlePosition()); // ���� ��ġ�� �ű��.
		pBullet->SetUpVector(GetUpVector(), GetRPDir(), GetRectPoint()); // ���� ����
		CreateObject(pBullet);

		if (false == tWeaponInfo.bIsInfinite) { // ������ �ƴ� ���
			--tWeaponInfo.iBulletCnt;
			if (tWeaponInfo.iBulletCnt <= 0) {
				SetWeaponState(false, GetCurWeaponType());
				ChangeNextWeapon();
				return;
			}
		}
	}
}

void CGTA_Player::Drive()
{
}

void CGTA_Player::Dead()
{
}

void CGTA_Player::GetInTheVehicle()
{
}

void CGTA_Player::GetOutTheVehicle()
{
}

void CGTA_Player::GetItem(CGTA_Item* pItem)
{
	// �������� ������
	if (E_ItemType::WEAPON == pItem->GetItemType()) {
		E_WeaponType eWeaponType = pItem->GetWeaponType();
		TWeaponInfo tWeaponInfo = GetWeaponInfo(eWeaponType);

		//�Ѿ� �߰�
		tWeaponInfo.iBulletCnt += pItem->GetWeaponInfo().iBulletCnt;

		// �������� �̹� ������ �ִٸ� �������� �Ѿ��� �����ش�.
		if (IsWeaponExists(eWeaponType)) {
			
			SetWeaponInfo(eWeaponType, tWeaponInfo);
		}
		// �������� ������ �� �������� ����Ű�� ���� ���� ��, ���� ����� �ٲ��ش�.
		else {
			SetWeaponState(true, eWeaponType); // ������ ���
			while (eWeaponType != GetCurWeaponType()) // ���� ����� �ٲ�
				ChangeNextWeapon();

			const TWeaponInfo& tCurWeaponInfo = m_vecWeapon[(UINT)m_eCurWeaponType].second;
			m_fAttackMaxCoolTime = tCurWeaponInfo.fShootCoolTime;
			m_fAttackCoolTime = tCurWeaponInfo.fShootCoolTime;
		}
	}
}

void CGTA_Player::ChangePrevWeapon()
{
	int iNextWeaponIdx = (int)m_eCurWeaponType + 1;
	while (true) {
		if (iNextWeaponIdx >= (int)E_WeaponType::END)
			iNextWeaponIdx = 0;

		if (true == m_vecWeapon[iNextWeaponIdx].first) {
			m_eCurWeaponType = (E_WeaponType)iNextWeaponIdx;
			break;
		}
		++iNextWeaponIdx;
	}



	const TWeaponInfo& tWeaponInfo = m_vecWeapon[(UINT)m_eCurWeaponType].second;
	m_fAttackMaxCoolTime = tWeaponInfo.fShootCoolTime;
	m_fAttackCoolTime = tWeaponInfo.fShootCoolTime;
}

void CGTA_Player::ChangeNextWeapon()
{
	int iPrevWeaponIdx = (int)m_eCurWeaponType - 1;
	while (true) {
		if (iPrevWeaponIdx < 0)
			iPrevWeaponIdx = (int)E_WeaponType::END - 1;

		if (true == m_vecWeapon[iPrevWeaponIdx].first) {
			m_eCurWeaponType = (E_WeaponType)iPrevWeaponIdx;
			break;
		}
		--iPrevWeaponIdx;
	}

	const TWeaponInfo& tWeaponInfo = m_vecWeapon[(UINT)m_eCurWeaponType].second;
	m_fAttackMaxCoolTime = tWeaponInfo.fShootCoolTime;
	m_fAttackCoolTime = tWeaponInfo.fShootCoolTime;
}