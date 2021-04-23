#pragma once
#include "CObject.h"

enum class E_CharacterState {
	idle,
	walk,
	hit,
	run,
	attack,
	getInTheCar,
	getOffTheCar,
	dead,
	stun,
};

enum class ECopState {
	trace,
	run,
};

enum class E_AIState {
	wander,
	runAway,
	trace
};

struct TCharacterInfo {
	float fHp;
	float fArmor;
	float fMoveSpeed;
	float fWalkSpeed;

	TCharacterInfo() : fHp(10.f), fArmor(0.f), fMoveSpeed(300.f), fWalkSpeed(100.f) {}
	void Save(FILE* _pFile);
	void Load(FILE* _pFile);
};


class CGTA_Vehicle;
class CGTA_PunchDetector;
class CGTA_AI;
class CPathFinding;
class CGTA_Item;
class CGTA_Character : public CObject
{
protected:
	TCharacterInfo m_tInfo;

	bool m_bIsDrive;
	bool m_bIsMoved;

	float m_fAttackCoolTime;
	float m_fAttackMaxCoolTime;

	float m_fStunCoolTime;
	float m_fStunMaxCoolTime;

	float m_fDeadCoolTime;
	float m_fDeadMaxCoolTime;

	float m_fRunawayDistance; // 총을 쏠 경우 일정 영역안에 있으면 도망치는 용도로 사용

	CGTA_Vehicle* m_pVehicle;

	CGTA_PunchDetector* m_pPunchDetector;

	E_WeaponType m_eCurWeaponType;
	vector<std::pair<bool, TWeaponInfo> > m_vecWeapon; // true : allow, false : not allow

	E_CharacterState m_eCharacterState; // 현재 캐릭터 상태
	E_AIState m_eAIState; // AI 상태

	// Component 
	CGTA_AI* m_pAI; // AI
	CPathFinding* m_pPathFinding;

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
	virtual void State();
	virtual void Stun();
	virtual void Move() {}
	virtual void Attack();
	virtual void Drive() {}
	virtual void Dead() {}
	virtual void GetInTheVehicle() {}
	virtual void GetOutTheVehicle() {}

	void ChangePrevWeapon();
	void ChangeNextWeapon();
	void GetItem(CGTA_Item* pItem);

	// AI
	virtual void MoveAI() {}
	virtual void Trace();
	virtual void Wander();
	virtual void Runaway();

	// AI
	void CreateAI();
	CGTA_AI* GetAI() { return m_pAI; } 

	void CreatePathFinding();
	CPathFinding* GetPathFinding() { return m_pPathFinding; }

	Vector3 GetNozzlePosition() { return GetUpVector() * 17.0f; }

	// Weapon
	bool IsWeaponExists(E_WeaponType _eWeaponType) { return m_vecWeapon[(UINT)_eWeaponType].first; }
	E_WeaponType GetCurWeaponType() { return m_eCurWeaponType; }
	const TWeaponInfo& GetWeaponInfo(E_WeaponType _eWeaponType) { return m_vecWeapon[(UINT)_eWeaponType].second; }
	void SetWeaponState(bool _bAllow, E_WeaponType _eWeaponType) { m_vecWeapon[(UINT)_eWeaponType].first = _bAllow; }

	void SetWeaponInfo(E_WeaponType _eWeaponType, const TWeaponInfo& _tWeaponInfo) { m_vecWeapon[(UINT)_eWeaponType].second = _tWeaponInfo; }

	void ActivePunchDetector(bool _bActive);


	// Character, AI State
	void SetCharacterState(E_CharacterState _eCharacterState) { m_eCharacterState = _eCharacterState; }
	E_CharacterState GetCharacterState() { return m_eCharacterState; }

	void SetAIState(E_AIState _eAIState) { m_eAIState = _eAIState; }
	E_AIState GetAIState() { return m_eAIState; }

	bool HaveGun() {
		if (E_WeaponType::FIST == GetCurWeaponType())
			return false;
		return true;
	}
	

	TCharacterInfo& CharacterInfo() { return m_tInfo; }

public:
	CLONE(CGTA_Character);

	CGTA_Character(const CGTA_Character& _origin);
public:
	CGTA_Character() = delete;
	CGTA_Character(E_GroupType _eGroupType);
	virtual ~CGTA_Character() override;
};