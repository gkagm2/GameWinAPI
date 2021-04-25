#pragma once
#include "CObject.h"
class CGTA_Bullet : public CObject
{
private:
	float m_fLifeCoolTime;
	float m_fLifeMaxCoolTime;
	float m_fSpeed;
	float m_fDamage;

	// TODO : ��ƼŬ �ؽ��� �ֱ�
	// Particle (�� ƨ���)
	// Particle (��, ��, ƨ���)

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

	virtual void OnCollisionEnter(CObject* _pOther) override;
	virtual void OnCollisionStay(CObject* _pOther) override;
	virtual void OnCollisionExit(CObject* _pOther) override;

	void SetDamage(float _fDamage) { m_fDamage = _fDamage; }
	float GetDamage() { return m_fDamage; }

public:
	CLONE(CGTA_Bullet);

	CGTA_Bullet(const CGTA_Bullet& _origin);
public:
	CGTA_Bullet() = delete;
	CGTA_Bullet(E_GroupType e_GroupType);
	virtual ~CGTA_Bullet();
	
};