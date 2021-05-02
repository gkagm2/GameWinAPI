#pragma once
#include "CObject.h"

class CSound;
class CGTA_Bullet : public CObject
{
private:
	CObject* m_pOwner; // 총을 쏜 주인

	float m_fLifeCoolTime;
	float m_fLifeMaxCoolTime;
	float m_fSpeed;
	float m_fDamage;

	// TODO : 파티클 텍스쳐 넣기
	CTexture* m_pBloodTex;
	CTexture* m_pSparkTex;

	// Particle (피 튕기기)
	// Particle (벽, 차, 튕기기)

public:
	virtual void Init();
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

	virtual void OnCollisionEnter(CObject* _pOther) override;
	virtual void OnCollisionStay(CObject* _pOther) override;
	virtual void OnCollisionExit(CObject* _pOther) override;

	void SetDamage(float _fDamage) { m_fDamage = _fDamage; }
	float GetDamage() { return m_fDamage; }

	void ChangeBloodTex();
	void ChangeSparkTex();

	void SetOwnerObj(CObject* _pOwner) { m_pOwner = _pOwner; }
	CObject* GetOwnerObj() { return m_pOwner; }

public:
	CLONE(CGTA_Bullet);

	CGTA_Bullet(const CGTA_Bullet& _origin);
public:
	CGTA_Bullet() = delete;
	CGTA_Bullet(E_GroupType e_GroupType, CObject* _pOwner);
	virtual ~CGTA_Bullet();
};