#pragma once
#include "CObject.h"
class CPlayer : public CObject
{
private:
	enum class E_UpgradeLevelType {
		LEVEL1,
		LEVEL2,
		LEVEL3,
		LEVEL4
	};
private:
	int m_iHp;

	float m_fSpeed;
	float m_fMissileSpeed;
	E_UpgradeLevelType m_eUpgradeLevel;

	float m_fFireCoolTime;
	float m_fFireMaxCoolTime;

public:
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void OnCollisionEnter(CObject* _pOther) override;
	virtual void Render(HDC _hDC) override;

private:
	void CreateMissile(float _fSpeed, Vector3 _vPosition, float _fDirAngle);
	void FireMissile();
	

public:
	CPlayer() = delete;
	CPlayer(E_GroupType _eGroupType);
	virtual ~CPlayer() override;
};

