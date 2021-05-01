#pragma once
#include "CGTA_Character.h"

class CGTA_Weapon;
class CGTA_Bullet;
class CGTA_Item;

class CGTA_Player : public CGTA_Character
{
private:
	bool m_bIsActiveAI;
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

	virtual void DriveUpdate() override;

	virtual void State() override;
	virtual void MoveUpdate() override;
	virtual void Attack() override;
	virtual void Dead() override;
	virtual void Drive() override;
	virtual void GetInTheVehicle() override;
	virtual void GetOutTheVehicle() override;
	virtual void HitByFist() override;
	virtual void InitAI() override;
	
	void SetActiveAI(bool _bIsActive) { m_bIsActiveAI = _bIsActive; }

	virtual void ChangePrevWeapon() override;
	virtual void ChangeNextWeapon() override;

public:
	CLONE(CGTA_Player);

public:
	CGTA_Player() = delete;
	CGTA_Player(E_GroupType _eGroupType);
	virtual ~CGTA_Player() override;
};