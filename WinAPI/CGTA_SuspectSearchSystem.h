#pragma once
#include "CObject.h"

enum class E_SuspectSearchLevel{
	none, // 수배가 안걸려있음
	level1,
	level2,
	level3,
	level4,
	level5,
	END
};

class CGTA_SuspectSearchSystem : public CObject
{
private:
	int m_iKillCnt;
	int m_iLevel;
	float m_fUpdateCoolTime;
	float m_fUpdateMaxCoolTime; 
	E_SuspectSearchLevel m_eSearchLevel;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	void ChangeLevel(E_SuspectSearchLevel _eLevel) { m_eSearchLevel = _eLevel; }
	E_SuspectSearchLevel GetCurLeve() { return m_eSearchLevel; }

	void PlusKillCnt() { ++m_iKillCnt; }
	void ResetKiilCnt() { m_iKillCnt = 0; ChangeLevel(E_SuspectSearchLevel::none); }

public:
	CLONE_DISABLE(CGTA_SuspectSearchSystem);

public:
	CGTA_SuspectSearchSystem(E_GroupType _eGroupType);
	virtual ~CGTA_SuspectSearchSystem() override;
};