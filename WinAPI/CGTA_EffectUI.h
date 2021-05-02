#pragma once
#include "CTextUI.h"
class CGTA_EffectUI : public CTextUI
{
private:
	float m_fOffCoolTime;
	float m_fOffMaxCoolTime;
	
	float m_fDestroyScale; // 삭제될 범위
	float m_fSpeed; // 확대 속도

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	void SetEffectPos(const Vector3& _vTargetPos);

public:
	CLONE(CGTA_EffectUI);

public:
	CGTA_EffectUI() = delete;
	CGTA_EffectUI(E_GroupType _eGroupType);
	virtual ~CGTA_EffectUI() override;
};