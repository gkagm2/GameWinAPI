#pragma once
#include "CObject.h"
#include "CMissile.h"
enum class E_MissileType;
class CMonster : public CObject
{
protected:
	float m_fSpeed;
	float m_fDirection; // using -1, 1
	float m_fRange;
	Vector3 m_vStartPosition;
	float m_fRatioX;
	float m_fRatioY;
	CObject* m_pTargetObj; // target to attack

	enum class E_UpgradeLevelType {
		LEVEL1,
		LEVEL2,
		LEVEL3,
		LEVEL4,
		END
	};

	float m_fHitEffectMaxCoolTime; // 맞을 때 스케일이 커졌다 작아지는 용도
	float m_fHitEffectCoolTime;
	float m_bIsHit; // 맞았는지 체크
	Vector3 m_vOriginalScale;

	float m_fMissileFireCoolTime;
	float m_fMissileMaxFireCoolTime;
	float m_fMissileSpeed;
	E_UpgradeLevelType m_eUpgradeLevel;


public:
	void SetRange(float _fRange) { m_fRange = _fRange; }
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }

public:	
	virtual void Update() override;
	virtual void OnCollisionEnter(CObject* _pOther) override;

public:
	void SetTargetObj(CObject* _pTargetObj) { m_pTargetObj = _pTargetObj; }

public:
	void SetStartPosition(Vector3 _vPosition) { m_vStartPosition = _vPosition; }
	void CreateMissile(float _fSpeed, Vector3 _vNozzlePosition, float _fDirAngle, E_MissileType _eMissileType);
	void FireMissile();

	void Move(int _type);

		
public:
	CMonster() = delete;
	CMonster(E_GroupType _eGroupType);
	virtual ~CMonster() override;
};

