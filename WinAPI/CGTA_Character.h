#pragma once
#include "CObject.h"

enum class E_CharacterState {
	idle,
	walk,
	hit,
	run,
	attack,
	dead,
	stun,
	drive,
	getInTheVehicle,
	getOutTheVehicle
};

enum class ECopState {
	trace,
	run,
};

enum class E_AIState {
	wander,
	runAway,
	trace,
	dead,
	stun,
	walkToCar,
	drive
};

struct TCharacterInfo {
	float fHp;
	float fArmor;
	float fMoveSpeed;
	float fWalkSpeed;

	const float fMaxHp;
	TCharacterInfo() : fHp(10.f), fArmor(0.f), fMoveSpeed(300.f), fWalkSpeed(60.f) , fMaxHp(10.f) {}
	void Save(FILE* _pFile);
	void Load(FILE* _pFile);
};

class CGTA_Vehicle;
class CGTA_AI;
class CPathFinding;
class CGTA_Item;
class CTexture;
class CSound;
class CGTA_Character : public CObject
{
protected:
	TCharacterInfo m_tInfo;

	bool m_bDrive;
	bool m_bIsMoved;

	float m_fAttackCoolTime;
	float m_fAttackMaxCoolTime;

	float m_fStunCoolTime;
	float m_fStunMaxCoolTime;

	float m_fDeadCoolTime;
	float m_fDeadMaxCoolTime;

	float m_fNoticeDistance; // 총을 쏠 경우 일정 영역안에 있으면 알아차리는 용도

	float m_fVehicleSearchDistance; // 차량을 찾는 최대 거리.

	CGTA_Vehicle* m_pVehicle;

	E_WeaponType m_eCurWeaponType;
	vector<std::pair<bool, TWeaponInfo> > m_vecWeapon; // true : allow, false : not allow

	E_CharacterState m_eCharacterState; // 현재 캐릭터 상태
	E_AIState m_eAIState; // AI 상태

	// Component 
	CGTA_AI* m_pAI; // AI
	CPathFinding* m_pPathFinding;

	// --- Resource ---
	CTexture* m_pNozzleTexture;

	// TODO : Delete
	// Sound
	CSound* pHasGotaGunSound[Sound_HasGotaGunSound_Len];
	CSound* pHelpSound[Sound_Help_Len];
	CSound* pOMGSound[Sound_OMG_Len];

protected:
	virtual void InitSound();

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

	virtual void DriveUpdate();
	virtual void MoveUpdate() {}

	virtual void State();
	virtual void Stun();
	virtual void Attack();
	void Attack(Vector3 _TargetPos);
	virtual void HitByFist();
	virtual void Drive();
	virtual void Dead();
	virtual void GetInTheVehicle();
	virtual void GetOutTheVehicle();

	virtual void ChangePrevWeapon();
	virtual void ChangeNextWeapon();
	void GetItem(CGTA_Item* pItem);

	float AutoTargeting(const Vector3& _vUpDirVec, const Vector3& _vTargetDirVec);

	// AI
	virtual void InitAI();
	virtual void MoveAI() {}
	virtual void Trace();
	virtual void Wander();
	virtual void Runaway();
	
	CGTA_Vehicle* FindNearbyVehicle(); // 못찾으면 nullptr 리턴

	// AI
	void CreateAI();
	CGTA_AI* GetAI() { return m_pAI; } 
	E_AIState GetCurAIState() { return m_eAIState; }

	void CreatePathFinding();
	CPathFinding* GetPathFinding() { return m_pPathFinding; }

	Vector3 GetNozzlePosition() { return GetUpVector() * 14.0f; }

	// Weapon
	bool IsWeaponExists(E_WeaponType _eWeaponType) { return m_vecWeapon[(UINT)_eWeaponType].first; }
	E_WeaponType GetCurWeaponType() { return m_eCurWeaponType; }
	const TWeaponInfo& GetWeaponInfo(E_WeaponType _eWeaponType) { return m_vecWeapon[(UINT)_eWeaponType].second; }
	void SetWeaponState(bool _bAllow, E_WeaponType _eWeaponType) { m_vecWeapon[(UINT)_eWeaponType].first = _bAllow; }

	void SetWeaponInfo(E_WeaponType _eWeaponType, const TWeaponInfo& _tWeaponInfo) { m_vecWeapon[(UINT)_eWeaponType].second = _tWeaponInfo; }
	void SelectWeapon(E_WeaponType _eWeaponType);
	
	void DropWeaponItem();

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

	float GetAttackMaxCoolTime() { return m_fAttackMaxCoolTime; }
	TCharacterInfo& CharacterInfo() { return m_tInfo; }

	void SetDrive(bool _bDrive) { m_bDrive = _bDrive; }
	bool AreYouDrive() { return m_bDrive; }

	/*CGTA_Vehicle* GetVehicle() { return m_pVehicle; }
	void SetVehicle(CGTA_Vehicle* _pVehicle) { m_pVehicle = _pVehicle; }*/

public:
	CLONE(CGTA_Character);

	CGTA_Character(const CGTA_Character& _origin);
public:
	CGTA_Character() = delete;
	CGTA_Character(E_GroupType _eGroupType);
	virtual ~CGTA_Character() override;
};