#pragma once
#include "CObject.h"
class CGTA_PunchDetector : public CObject
{
private:
	CObject* m_pOwnerObj;
	float m_fPunchCoolTime;
	float m_fPunchMaxCoolTime;

	Vector3 m_fFinalPos;
	Vector3 m_fOffsetPos;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

	virtual void OnCollisionEnter(CObject* _pOther) override;

public:
	const Vector3& GetFinalPosition() { return m_fFinalPos; }
	const Vector3& GetOffsetPosition() { return m_pOwnerObj->GetUpVector() * 10.f; }

public:
	CLONE(CGTA_PunchDetector);

public:
	CGTA_PunchDetector() = delete;
	CGTA_PunchDetector(E_GroupType _eGroupType, CObject* _OwnerObj);
	virtual ~CGTA_PunchDetector() override;
};