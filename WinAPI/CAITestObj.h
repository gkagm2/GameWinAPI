#pragma once
#include "CObject.h"
class CGTA_AI;
class CPathFinding;
class CAITestObj : public CObject
{
private:
	Vector3 m_vTargetPos;
	CGTA_AI* m_pAI;
	CPathFinding* m_pPathFinding;

	float m_fPathFindCoolTime;
	float m_fPathFindMaxCoolTime;
	bool m_bIsPathFind;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

	virtual void OnCollisionEnter(CObject* _pOther);
	virtual void OnCollisionStay(CObject* _pOther);
	virtual void OnCollisionExit(CObject* _pOther);
	
	bool IsArrivedDestination();
	void CreatePathFinding();

public:
	CLONE(CAITestObj);
public:
	CAITestObj() = delete;
	CAITestObj(E_GroupType _eGroupType);
	virtual ~CAITestObj() override;
};