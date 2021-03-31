#pragma once
#include "CObject.h"

class CMonster;
class CMonster_Small;
class CMonsterRespawner : public CObject
{
private:
	vector<CMonster_Small*> m_vecSmallMonster;
	vector<CMonster*> m_vecMonster;

	float respawnMaxCoolTime;
	float respawnCoolTime;
	int respawnCnt = 0;



	int m_iPhase;

public:
	virtual void Update() override;
	virtual void LateUpdate() override;


public:
	void CreateSmallMonster();
	void CreateMonster();
	void InitRespawnMonster();

private:
	bool IsAllDeadSmallMonster();
	bool IsAllDeadMonster();


public:
	CLONE(CMonsterRespawner);

public:
	CMonsterRespawner() = delete;
	CMonsterRespawner(E_GroupType _eGroupType);
	virtual ~CMonsterRespawner() override;
};

