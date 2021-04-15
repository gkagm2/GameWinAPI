#pragma once
#include "CGTA_Character.h"
#include "CGTA_Weapon.h"

class CGTA_Weapon;
class CGTA_Bullet;

class CGTA_Player : public CGTA_Character
{
private:
	bool m_bIsDrive;
	CGTA_Weapon m_cWeapon;
	CGTA_Bullet* m_pBulletPref;

	float m_fAttackCoolTime;
	float m_fAttackMaxCoolTime;
	
public:
	virtual void Init() override;
	virtual void PrevUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

	virtual void OnCollisionEnter(CObject* _pOther);
	virtual void OnCollisionStay(CObject* _pOther);
	virtual void OnCollisionExit(CObject* _pOther);

public:
	virtual void Move() override;
	virtual void Attack() override;
	virtual void Drive() override;
	virtual void Dead() override;
	virtual void GetInTheVehicle() override;
	virtual void GetOutTheVehicle() override;

	

	void ChangePrevWeapon();
	void ChangeNextWeapon();

public:
	CLONE(CGTA_Player);

public:
	CGTA_Player() = delete;
	CGTA_Player(E_GroupType _eGroupType);
	virtual ~CGTA_Player() override;
};