#pragma once
#include "CGTA_Character.h"

class CGTA_Weapon;
class CGTA_Bullet;
class CGTA_Item;

class CGTA_Player : public CGTA_Character
{
private:
	
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
	virtual void State() override;
	virtual void Move() override;
	virtual void Attack() override;
	virtual void Dead() override;
	virtual void Drive() override;
	virtual void GetInTheVehicle() override;
	virtual void GetOutTheVehicle() override;
	virtual void HitByFist() override;

public:
	CLONE(CGTA_Player);

public:
	CGTA_Player() = delete;
	CGTA_Player(E_GroupType _eGroupType);
	virtual ~CGTA_Player() override;
};