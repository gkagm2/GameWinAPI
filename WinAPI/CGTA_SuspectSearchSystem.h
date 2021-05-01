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
	int m_iKiilCnt;
	int m_iLevel;
	float m_fInitCoolTime;
	float m_fInitMaxCoolTime; 
	E_SuspectSearchLevel m_eSearchLevel;
	
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	void ChangeLevel(E_SuspectSearchLevel _eLevel);
	void GetCurLeve() {}

public:
	CLONE_DISABLE(CGTA_SuspectSearchSystem);

public:
	CGTA_SuspectSearchSystem(E_GroupType _eGroupType);
	virtual ~CGTA_SuspectSearchSystem() override;
};